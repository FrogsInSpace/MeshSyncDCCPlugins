#pragma once
#include <unordered_map>
#include "pch.h"
#include "MeshUtils/muMath.h"
#include "DNA_object_types.h"
#include "MeshSync/SceneGraph/msMesh.h"
#include <DNA_node_types.h>
#include "MeshSyncClient/msInstancesManager.h"
#include "MeshUtils/muMath.h"

namespace blender {

#if BLENDER_VERSION >= 300
	class GeometryNodesUtils
	{
	public:
        /// <summary>
        /// /// Converts the world matrix from blender to Unity coordinate system
        /// /// </summary>
		static mu::float4x4& blenderToUnityWorldMatrix(mu::float4x4& blenderMatrix);

		/// <summary>
		/// Will invoke f for every instance. The first argument of f is the name
		/// of the mesh that is being instantiated and the second argumenet is the world matrix
		/// of the instance in the Unity3D Coordinate system
		/// </summary>
		/// <param name="f"></param>
		static void foreach_instance(std::function<void (std:: string, mu::float4x4)> f);

		static void foreach_instance(std::function<void(std::string, std::vector<mu::float4x4>)> f);

		/// <summary>
		/// Converts a name with the type embedded
		/// to the name used without in the outliner window
		/// i.e. MEMyMesh -> MyMesh
		/// </summary>
		/// <param name=""></param>
		/// <returns></returns>
		static std::string getOutlinerName(char *name);

		void setInstancesDirty(bool dirty);
		bool getInstancesDirty();

	private:
		bool m_instances_dirty;
	};
#endif
}

