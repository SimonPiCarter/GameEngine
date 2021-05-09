#include <gtest/gtest.h>

#include <message/LogicMessageHandler.h>

#include <thread>

namespace
{

/// @brief this class will just add an element in vect_p when deleted
class TestNoOp : public NoOpLogicMessage
{
public:
	TestNoOp(int index_p, std::vector<int> &vect_p) : NoOpLogicMessage(""), _index(index_p), _vect(vect_p) {}
	~TestNoOp()
	{
		_vect.push_back(_index);
	}

private:
	int const _index;
	std::vector<int> &_vect;
};

void postMessage(LogicMessageHandler &handler_p, std::vector<int> &vect_p)
{
	for(size_t i = 0; i < 1000 ; ++ i)
	{
		handler_p.registerMessage(new TestNoOp(i, vect_p));
	}
}

void handleMessage(LogicMessageHandler &handler_p, bool &end_p)
{
	while(!end_p || handler_p.hasMessage())
	{
		while(handler_p.popMessage()) {}
		handler_p.swapMessages();
	}
}

}

TEST(message, simple)
{
	LogicMessageHandler handler_l;
	std::vector<int> vect_l;

	bool end_l = false;

	std::thread t1_l(postMessage, std::ref(handler_l), std::ref(vect_l));
	std::thread t2_l(handleMessage, std::ref(handler_l), std::ref(end_l));

	t1_l.join();
	end_l = true;
	t2_l.join();

	ASSERT_EQ(1000, vect_l.size());
	for(size_t i = 0; i < 1000 ; ++ i)
	{
		EXPECT_EQ(i, vect_l[i]) << "index " << i << " was expected to be " << i << " but was " << vect_l[i];
	}
}

