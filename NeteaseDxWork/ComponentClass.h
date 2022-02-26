#pragma once
#include <memory>

class Transform;
class ModelObject;
class DirectionLightObject;
class CameraObject;
class TPCameraObject;

typedef std::shared_ptr<Transform> TransformPtr;
typedef std::shared_ptr<ModelObject> ModelObjectPtr;
typedef std::shared_ptr<DirectionLightObject> DirectionLightObjectPtr;
typedef std::shared_ptr<CameraObject> CameraObjectPtr;
typedef std::shared_ptr<TPCameraObject> TPCameraObjectPtr;