#include "camera.h"
#define MAYA_EXPORT
#include "CommonDeclaration.h"

MStatus Camera::exportCamera(MFnCamera& mayaCamera, camera& camera, map<const char*, int>& transformHeiraki, vector<const char*>& names, vector<vector<unsigned int>>& parentIDs)
{
	MStatus status;

	// Check if the camera is orthogonal or not
	if (mayaCamera.isOrtho())
		camera.projection = camera.eOrthogonal;
	else
		camera.projection = camera.ePerspective;

	// Get the data we need from the camera
	camera.name_length = mayaCamera.name().length();
	names.push_back(mayaCamera.name().asChar());

	MGlobal::displayInfo(MString() + names[names.size() - 1]);

	// Position
	camera.position[0] = mayaCamera.eyePoint().x;
	camera.position[1] = mayaCamera.eyePoint().y;
	camera.position[2] = mayaCamera.eyePoint().z;

	// Camera Up Direction
	MGlobal::displayInfo(MString() + mayaCamera.upDirection().x + "   " + mayaCamera.upDirection().y + "   " + mayaCamera.upDirection().z);

	camera.up_vector[0] = mayaCamera.upDirection().x;
	camera.up_vector[1] = mayaCamera.upDirection().y;
	camera.up_vector[2] = mayaCamera.upDirection().z;

	// Camera look at
	MGlobal::displayInfo(MString() + mayaCamera.viewDirection().x + "   " + mayaCamera.viewDirection().y + "   " + mayaCamera.viewDirection().z);

	camera.interest_position[0] = mayaCamera.viewDirection().x;
	camera.interest_position[1] = mayaCamera.viewDirection().y;
	camera.interest_position[2] = mayaCamera.viewDirection().z;

	// FOV
	camera.field_of_view_x = mayaCamera.horizontalFieldOfView(&status);
	camera.field_of_view_y = mayaCamera.verticalFieldOfView(&status);

	MGlobal::displayInfo(MString() + "FOV_X: " + camera.field_of_view_x + "   FOV_Y: " + camera.field_of_view_y);

	// Clip Planes
	camera.near_plane = mayaCamera.nearClippingPlane(&status);
	camera.far_plane = mayaCamera.farClippingPlane(&status);

	MGlobal::displayInfo(MString() + "near_plane: " + camera.near_plane + "   far_plane: " + camera.far_plane);

	// Parent
	vector<unsigned int> pIDs(mayaCamera.parentCount());
	camera.nrOfParents = mayaCamera.parentCount();
	for (unsigned int i = 0; i < mayaCamera.parentCount(); i++)
	{
		MObject parent = mayaCamera.parent(i);
		MFnDagNode storeParent(parent);
		pIDs[i] = transformHeiraki[storeParent.name().asChar()];
	}
	parentIDs.push_back(pIDs);

	return status;
}