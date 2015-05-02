#include "Nurb.h"
#include "maya/mPlug.h"

MStatus exportNurb::exportNurbSphere(MFnNurbsSurface& mayaNurbSphere, Nurb& nurbSphere, map<const char*, int> transformHierarchy, vector<const char*>& name, vector<vector <int> >& parentID)
{
	MS status;

	MPlug aPlug = mayaNurbSphere.findPlug("create", true, &status);

	//om findPlug hittar create så fortsätt 
	if (status == MS::kSuccess)
	{

		//nurbSphere.name = mayaNurbSphere.name().asChar();
		name.push_back(mayaNurbSphere.name().asChar());
		nurbSphere.name_Length = mayaNurbSphere.name().length();

		//oneLight.name = mayaLight.name().asChar();
		//Lheader.name_Length = mayaLight.name().length();

		// ---------------------------------------------------//
		//					RADIUS                            //
		// ---------------------------------------------------//


		//makeNurbSphere är kopplad till en plug som heter create (Create finns i dagnoden nurbsphereshape)

		MPlugArray allConnections;
		MObject object;

		//Kollar alla plugs som är relaterad till plug:en Create
		aPlug.connectedTo(allConnections, true, false);

		for (unsigned int i = 0; i < allConnections.length(); i++)
		{
			object = allConnections[i].node();

			MFnDependencyNode makeSphere(object);
			//Kollar om radius finns i någon av de relaterade plug:en
			nurbSphere.radius = makeSphere.findPlug("radius").asDouble();

			MGlobal::displayInfo(MString() + "MakeSphere Name: " + makeSphere.name());
			MGlobal::displayInfo(MString() + "Nurb Radius: " + nurbSphere.radius);

		}


		// ---------------------------------------------------//
		//					PARENT ID                         //
		// ---------------------------------------------------//

		//nurbSphere.parentID = new int[mayaNurbSphere.parentCount()];
		vector<int> parentIndex(mayaNurbSphere.parentCount());

		for (unsigned int i = 0; i < mayaNurbSphere.parentCount(); i++)
		{
			MObject parent = mayaNurbSphere.parent(i);
			MFnDagNode storeParent(parent);
			//nurbSphere.parentID[i] = transformHierarchy[storeParent.name().asChar()];
			parentIndex[i] = transformHierarchy[storeParent.name().asChar()];

			//MGlobal::displayInfo(MString() + "PARENT ID: " + nurbSphere.parentID[i]);

		}

		nurbSphere.numberOfParent = mayaNurbSphere.parentCount();
		parentID.push_back(parentIndex);
	}



	/// --- 
	/*MPlug test = mayaNurbSphere.findPlug("create", true);

	MPlugArray allConnections;
	MObject object;

	test.connectedTo(allConnections, true, false);

	for (unsigned int i = 0; i < allConnections.length(); i++)
	{
		object = allConnections[i].node();

			MFnDependencyNode makeSphere(object);
			double twoort = makeSphere.findPlug("radius").asDouble();
			MGlobal::displayInfo(MString() + "nwediuewhdukewhdiukehuh " + makeSphere.name());
			MGlobal::displayInfo(MString() + "nwediuewhdukewhdiukehuh " + twoort);

			
	
	}*/

	



	return status;
	
	


}