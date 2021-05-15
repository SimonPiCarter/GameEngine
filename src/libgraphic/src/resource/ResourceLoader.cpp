#include "ResourceLoader.h"

#include "OgreMesh.h"
#include "OgreMeshManager.h"
#include "OgreMeshManager2.h"
#include "OgreMesh2Serializer.h"

namespace
{
Ogre::MeshPtr loadV1Mesh(Resource const &res_p)
{
	Ogre::v1::MeshPtr v1Mesh;
	Ogre::MeshPtr v2Mesh;

	//---------------------------------------------------------------------------------------
	//Import to v2
	//---------------------------------------------------------------------------------------

	//Load the v1 mesh. Notice the v1 namespace
	//Also notice the HBU_STATIC flag; since the HBU_WRITE_ONLY
	//bit would prohibit us from reading the data for importing.
	v1Mesh = Ogre::v1::MeshManager::getSingleton().load(
				res_p._meshPath, Ogre::ResourceGroupManager::AUTODETECT_RESOURCE_GROUP_NAME,
				Ogre::v1::HardwareBuffer::HBU_STATIC, Ogre::v1::HardwareBuffer::HBU_STATIC );

	//Create a v2 mesh to import to, with a different name (arbitrary).
	v2Mesh = Ogre::MeshManager::getSingleton().createManual(
				res_p._meshPath+"v2", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME );

	bool halfPosition   = true;
	bool halfUVs        = true;
	bool useQtangents   = true;

	//Import the v1 mesh to v2
	v2Mesh->importV1( v1Mesh.get(), halfPosition, halfUVs, useQtangents );

	//We don't need the v1 mesh. Free CPU memory, get it out of the GPU.
	//Leave it loaded if you want to use athene with v1 Entity.
	v1Mesh->unload();

	return v2Mesh;
}
Ogre::MeshPtr loadV2Mesh(Resource const &res_p)
{
	Ogre::MeshPtr v2Mesh;

	v2Mesh = Ogre::MeshManager::getSingleton().load(
				res_p._meshPath, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME );

	return v2Mesh;
}
}

Ogre::MeshPtr loadMesh(Resource const &res_p)
{
	std::string meshPath_l = res_p._meshPath;
	if(res_p._v1Mesh)
	{
		meshPath_l += "v2";
	}
	Ogre::MeshPtr meshptr_l = Ogre::MeshManager::getSingleton().getByName(meshPath_l);
	if(!meshptr_l.isNull())
	{
		return meshptr_l;
	}
	if(res_p._v1Mesh)
	{
		return loadV1Mesh(res_p);
	} else
	{
		return loadV2Mesh(res_p);
	}
}
