#pragma once
#include "lib3mf_implicit.hpp"

sLib3MFPosition fnCreateVertex(const float x, const float y, const float z)
{
	return { x, y, z };
}

sLib3MFTriangle fnCreateTriangle(const unsigned int v0, const unsigned int v1, const unsigned int v2)
{
	return { v0, v1, v2 };
}
sLib3MFTriangleProperties fnCreateTriangleColor(Lib3MF::PColorGroup colorGroup, Lib3MF_uint32 colorID1, Lib3MF_uint32 colorID2, Lib3MF_uint32 colorID3)
{
	sLib3MFTriangleProperties sTriangleProperty;
	sTriangleProperty.m_ResourceID = colorGroup->GetResourceID();
	sTriangleProperty.m_PropertyIDs[0] = colorID1;
	sTriangleProperty.m_PropertyIDs[1] = colorID2;
	sTriangleProperty.m_PropertyIDs[2] = colorID3;
	return sTriangleProperty;
}


void delete_3mf_output()
{
	std::string file{ "3MF_output/output.3mf" };
	std::filesystem::remove(file);
}

void make_3mf_file(Skin& skin, std::vector<std::vector<StlTemplates::Facet>>& input_triangles)
{
	Lib3MF::PWrapper wrapper = wrapper->loadLibrary();
	Lib3MF::PModel model = wrapper->CreateModel();

	Lib3MF::PColorGroup colorGroup = model->AddColorGroup();

	std::vector<sLib3MFPosition> output_vertices;
	std::vector<sLib3MFTriangle> output_triangles;
	std::vector<sLib3MFTriangleProperties> output_colors;
	size_t vertices_iter{ 0 };

	Lib3MF::PMeshObject meshObject = model->AddMeshObject();
	meshObject->SetName("Object");

	for (int i{ 0 }; i < skin.colors.size(); i++)
		if (skin.colors[i][3] != 0)
		{
			auto color{ colorGroup->AddColor(wrapper->RGBAToColor(skin.colors[i][0], skin.colors[i][1], skin.colors[i][2], skin.colors[i][3]))};
			auto TriangleColor{ fnCreateTriangleColor(colorGroup, color, color, color) };
			
			for (auto& vert : input_triangles[i])
			{	
				output_vertices.push_back(fnCreateVertex(vert.coordinates[0].x, vert.coordinates[0].y, vert.coordinates[0].z));
				output_vertices.push_back(fnCreateVertex(vert.coordinates[1].x, vert.coordinates[1].y, vert.coordinates[1].z));
				output_vertices.push_back(fnCreateVertex(vert.coordinates[2].x, vert.coordinates[2].y, vert.coordinates[2].z));
				
				output_triangles.push_back(fnCreateTriangle(vertices_iter, vertices_iter+1, vertices_iter+2));
				output_colors.push_back(TriangleColor);
				vertices_iter += 3;
			}
		}
	meshObject->SetGeometry(output_vertices, output_triangles);
	for (int i{ 0 }; i < output_triangles.size(); i++)
		meshObject->SetTriangleProperties(i, output_colors[i]);
	
	// No idea what this line does, even after looking at the docs, but it's supposed to be here...
	
	// Object Level Property
	auto color_property{ colorGroup->AddColor(wrapper->RGBAToColor(0, 0, 0, 0)) };
	auto TriangleColorProperty{ fnCreateTriangleColor(colorGroup, color_property, color_property, color_property) };
	meshObject->SetObjectLevelProperty(TriangleColorProperty.m_ResourceID, TriangleColorProperty.m_PropertyIDs[0]);

	// Add build item
	model->AddBuildItem(meshObject.get(), wrapper->GetIdentityTransform());

	Lib3MF::PWriter writer{ model->QueryWriter("3mf") };
	writer->WriteToFile("output.3mf");


}
