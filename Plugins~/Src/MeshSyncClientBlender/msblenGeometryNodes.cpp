#include "msblenGeometryNodes.h"
#include "BlenderPyObjects/BlenderPyDepsgraphUpdate.h"
#include <sstream>
#include <BlenderPyObjects/BlenderPyContext.h>
#include <MeshSync/SceneGraph/msMesh.h>
#include "BlenderPyObjects/BlenderPyNodeTree.h"
#include "BlenderPyObjects/BlenderPyScene.h"
#include <msblenUtils.h>


using namespace std;
using namespace mu;

namespace blender {

#if BLENDER_VERSION >= 300
    /// <summary>
    /// Converts the world matrix from blender to Unity coordinate systems
    /// </summary>
    /// <param name="blenderMatrix"></param>
    /// <returns></returns>
    float4x4& GeometryNodesUtils::blenderToUnityWorldMatrix(float4x4& blenderMatrix) {

        auto rotation = rotate_x(-90 * DegToRad);
        auto rotation180 = rotate_z(180 * DegToRad);
        auto scale = float3::one();
        scale.x = -1;

        auto result =
            to_mat4x4(rotation) *
            scale44(scale) *
            blenderMatrix *
            to_mat4x4(rotation) *
            to_mat4x4(rotation180) *
            scale44(scale);

        return result;
    }


    void GeometryNodesUtils::foreach_instance(std::function<void(string, float4x4)> f)
    {

        auto blContext = blender::BlenderPyContext::get();
        // Build a map between data and the objects they belong to
        auto scene = blContext.scene();
        auto mscene = BlenderPyScene(scene);

        map<string, Object*> dataToObject;

        mscene.each_objects([&](Object* obj) {
            if (obj == nullptr || obj->data == nullptr)
                return;
            auto data_id = (ID*)obj->data;
            dataToObject[data_id->name] = obj;
            });

        // BlenderPyContext is an interface between depsgraph operations and anything that interacts with it
        auto depsgraph = blContext.evaluated_depsgraph_get();

        // Iterate over the object instances collection of depsgraph
        CollectionPropertyIterator it;

        blContext.object_instances_begin(&it, depsgraph);

        for (; it.valid; blContext.object_instances_next(&it)) {
            // Get the instance as a Pointer RNA.
            auto instance = blContext.object_instances_get(&it);

            // Get the object that the instance refers to
            auto instance_object = blContext.instance_object_get(instance);


            // If the object is null, skip
            if (instance_object == nullptr)
                continue;

            // if the instance is not an instance, skip
            if (!blContext.object_instances_is_instance(instance))
                continue;

            auto object = blContext.object_get(instance);

            if (object->type != OB_MESH)
                continue;

            auto data_id = (ID*)object->data;
            auto hierarchyObject = dataToObject[data_id->name];
            if (hierarchyObject == nullptr)
                continue;

            auto path = get_path(hierarchyObject);

            auto world_matrix = float4x4();
            blContext.world_matrix_get(&instance, &world_matrix);

            auto result = blenderToUnityWorldMatrix(world_matrix);

            
            f(move(path), move(result));
        }

        // Cleanup resources
        blContext.object_instances_end(&it);
    }

    void GeometryNodesUtils::foreach_instance(std::function<void(string, vector<float4x4>)> f) {
        map<string, vector<float4x4>> map;
        foreach_instance([&](string name, float4x4 matrix) {
            map[name].push_back(matrix);
            });

        for (auto& entry : map) {
            f(entry.first, entry.second);
        }
    }

    void GeometryNodesUtils::setInstancesDirty(bool dirty)
    {
        m_instances_dirty = dirty;
    }
    bool GeometryNodesUtils::getInstancesDirty()
    {
        return m_instances_dirty;
    }
#endif
}