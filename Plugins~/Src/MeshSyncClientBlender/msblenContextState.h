#include <MeshSyncClient/msTransformManager.h>
#include "MeshSync/MeshSyncMacros.h"

class msblenContextState {
public:
    // note:
// ObjectRecord and Blender's Object is *NOT* 1 on 1 because there is 'dupli group' in Blender.
// dupli group is a collection of nodes that will be instanced.
// so, only the path is unique. Object maybe shared by multiple ObjectRecord.
    struct ObjectRecord {
        MS_CLASS_DEFAULT_NOCOPY_NOASSIGN(ObjectRecord);
            //std::vector<NodeRecord*> branches; // todo

            std::string path;
            std::string name;
            Object* host = nullptr; // parent of dupli group
            Object* obj = nullptr;
            Bone* bone = nullptr;

            ms::TransformPtr dst;

            bool touched = false;
            bool renamed = false;

            void clearState();
    };

    msblenContextState(ms::TransformManager& manager) : manager(manager) 
    {
    };

    std::set<const Object*> pending;
    std::map<const void*, msblenContextState::ObjectRecord> records;
    std::map<Bone*, ms::TransformPtr> bones;

    ms::TransformManager& manager;

    template<typename T>
    T& GetManager() { return (T&)manager; }

    void msblenContextState::eraseObjectRecords();
    void msblenContextState::eraseStaleObjects();
    void clear();
    void clearRecordsState();

    msblenContextState::ObjectRecord& msblenContextState::touchRecord(const Object* obj, const std::string& base_path = "", bool children = false);
};