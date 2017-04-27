#include "Cube.h"

#include "Geometry/MeshData.h"

float _rotation;

std::vector<tue::fvec3> c_vertices;
std::vector<tue::fvec4> c_colors;
std::vector<std::uint8_t> c_indices;

void Cube_setupGL(double width, double height)
{
    // Initialize GL state.
    glDisable(GL_DITHER);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_FASTEST);
    glClearColor(1.0f, 0.41f, 0.71f, 1.0f);
    glEnable(GL_CULL_FACE);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_DEPTH_TEST);

    glViewport(0, 0, width, height);
    GLfloat ratio = (GLfloat)width / height;
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustumf(-ratio, ratio, -1, 1, 1, 10);

	// Setup cube:
    {
		auto meshData = TueMeshGenerator::CreateBox(1.0f, 1.0f, 1.0f, 0);

		// Vertices:
		{
			const auto& vertices = meshData.Vertices;
			c_vertices.resize(vertices.size());
			for (std::size_t i = 0; i < vertices.size(); ++i)
				c_vertices[i] = vertices[i].Position;
		}

		// Colors
		{
			c_colors.resize(meshData.Vertices.size());
			for (std::size_t i = 0; i < c_colors.size(); ++i)
				c_colors[i] = { 0.0f, 0.0f, 1.0f, 1.0f };
		}

		// Indices
		{
			const auto& indices = meshData.Indices;
			c_indices.resize(indices.size());
			for (std::size_t i = 0; i < indices.size(); ++i)
				c_indices[i] = indices[i];
		}
    }
}

void Cube_tearDownGL()
{
}

void Cube_update()
{
    _rotation += 1.f;

}
void Cube_prepare()
{
     glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Cube_draw()
{
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(0, 0, -3.0f);
    glRotatef(_rotation * 0.25f, 1, 0, 0);  // X
    glRotatef(_rotation, 0, 1, 0);          // Y

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);

    glFrontFace(GL_CW);
	glVertexPointer(3, GL_FLOAT, 0, c_vertices.data());
	glColorPointer(4, GL_FLOAT, 0, c_colors.data());
	glDrawElements(GL_TRIANGLES, c_indices.size(), GL_UNSIGNED_BYTE, c_indices.data());
}
