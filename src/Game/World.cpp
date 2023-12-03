#include "stdafx.h"
#include "World.h"
#include "Viewport.h"
//-----------------------------------------------------------------------------
namespace
{
	const char* vsText = R"(
layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec3 aColor;
layout(location = 3) in vec2 aTexCoord;

uniform mat4 uWorld;
uniform mat4 uView;
uniform mat4 uProjection;

out vec3 FragmentPosition;
out vec3 Normal;
out vec2 TexCoords;
out vec3 Color;

void main()
{
	// Rotate the object normals by a 3x3 normal matrix.
	// We could also do this CPU-side to avoid doing it per-vertex
	mat3 normalMatrix = transpose(inverse(mat3(uWorld)));

	// output
	FragmentPosition = vec3(uWorld * vec4(aPosition, 1.0));
	Normal = normalize(normalMatrix * aNormal);
	TexCoords = aTexCoord;
	Color = aColor;
	gl_Position = uProjection * uView * vec4(FragmentPosition, 1.0);
}
)";

	constexpr const char*fsText = R"(
struct Material {
	sampler2D diffuse;
	vec3 specular;
	float shininess;
};
struct Light {
	vec3 direction;
  
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

out vec4 FragmentColor;

in vec3 FragmentPosition;
in vec3 Normal;
in vec2 TexCoords;
in vec3 Color;

uniform vec3 uViewPosition;
uniform Material uMaterial;
uniform Light uLight;

void main()
{
	// compute diffuse material
	vec4 diffuseColor = texture(uMaterial.diffuse, TexCoords);
	if (diffuseColor.a < 0.02) discard;
	diffuseColor = diffuseColor * vec4(Color, 1.0);

	// ambient
	vec3 ambient = uLight.ambient * diffuseColor.rgb;

	// diffuse 
	vec3 lightDir = normalize(-uLight.direction);
	float diff = max(dot(Normal, lightDir), 0.0);
	vec3 diffuse = uLight.diffuse * diff * texture(uMaterial.diffuse, TexCoords).rgb;

	// specular
	vec3 viewDir = normalize(uViewPosition - FragmentPosition);
	vec3 reflectDir = reflect(-lightDir, Normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), uMaterial.shininess);
	vec3 specular = uLight.specular * spec * uMaterial.specular;

	FragmentColor = vec4(ambient + diffuse + specular, 1.0);
}
)";

	float vertices[] =
	{
		// positions          // normals           // texture coords // color
		   -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, 1.0f,  0.0f,  0.0f,
			0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, 1.0f,  1.0f,  0.0f,
			0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, 1.0f,  1.0f,  1.0f,
			0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, 1.0f,  1.0f,  1.0f,
		   -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, 1.0f,  0.0f,  1.0f,
		   -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, 1.0f,  0.0f,  0.0f,

		   -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, 1.0f,  0.0f,  0.0f,
			0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, 1.0f,  1.0f,  0.0f,
			0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, 1.0f,  1.0f,  1.0f,
			0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, 1.0f,  1.0f,  1.0f,
		   -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, 1.0f,  0.0f,  1.0f,
		   -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, 1.0f,  0.0f,  0.0f,

		   -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f, 1.0f,  1.0f,  0.0f,
		   -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f, 1.0f,  1.0f,  1.0f,
		   -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f, 1.0f,  0.0f,  1.0f,
		   -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f, 1.0f,  0.0f,  1.0f,
		   -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f, 1.0f,  0.0f,  0.0f,
		   -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f, 1.0f,  1.0f,  0.0f,

			0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f, 1.0f, 1.0f, 1.0f,  1.0f,  0.0f,
			0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f, 1.0f, 1.0f, 1.0f,  1.0f,  1.0f,
			0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f, 1.0f, 1.0f, 1.0f,  0.0f,  1.0f,
			0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f, 1.0f, 1.0f, 1.0f,  0.0f,  1.0f,
			0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f, 1.0f, 1.0f, 1.0f,  0.0f,  0.0f,
			0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f, 1.0f, 1.0f, 1.0f,  1.0f,  0.0f,

		   -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f, 1.0f, 1.0f, 1.0f,  0.0f,  1.0f,
			0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f, 1.0f, 1.0f, 1.0f,  1.0f,  1.0f,
			0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f, 1.0f, 1.0f, 1.0f,  1.0f,  0.0f,
			0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f, 1.0f, 1.0f, 1.0f,  1.0f,  0.0f,
		   -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f, 1.0f, 1.0f, 1.0f,  0.0f,  0.0f,
		   -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f, 1.0f, 1.0f, 1.0f,  0.0f,  1.0f,

		   -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f, 1.0f, 1.0f, 1.0f,  0.0f,  1.0f,
			0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f, 1.0f, 1.0f, 1.0f,  1.0f,  1.0f,
			0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f, 1.0f, 1.0f, 1.0f,  1.0f,  0.0f,
			0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f, 1.0f, 1.0f, 1.0f,  1.0f,  0.0f,
		   -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f, 1.0f, 1.0f, 1.0f,  0.0f,  0.0f,
		   -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f, 1.0f, 1.0f, 1.0f,  0.0f,  1.0f,
	};

	ShaderProgramRef spTile;
	GeometryBufferRef geom;
	Texture2DRef diffuse;

	StaticModelRef model;
}
//-----------------------------------------------------------------------------
bool World::Create()
{
	auto& renderSystem = GetRenderSystem();


	spTile = renderSystem.CreateShaderProgram({ vsText }, { fsText });

	geom = renderSystem.CreateGeometryBuffer(BufferUsage::StaticDraw, static_cast<unsigned>(Countof(vertices)) / 11, static_cast<unsigned>(sizeof(float)) * 11, vertices, spTile);

	Texture2DInfo texInfo;
	texInfo.magFilter = TextureMagFilter::Nearest;
	texInfo.minFilter = TextureMinFilter::NearestMipmapNearest;
	texInfo.mipmap = false;
	//texInfo.verticallyFlip = true;
	diffuse = renderSystem.CreateTexture2D("../Data/Textures/testTop.png", true, texInfo);



	{
		model = GetGraphicsSystem().CreateModel("../Data/Models/box.obj");

		std::vector<StaticMeshVertex> vert;
		for (int i = 0; i < model->subMeshes[0].indices.size(); i++)
		{
			auto index = model->subMeshes[0].indices[i];
			StaticMeshVertex v = model->subMeshes[0].vertices[index];
			vert.push_back(v);
		}


		for (int i = 0; i < vert.size(); i++)
		{
			std::string s = "";
		}
	}

	if (!m_map.Create())
		return false;

	return true;
}
//-----------------------------------------------------------------------------
void World::Destroy()
{
	spTile.reset();
	geom.reset();
	diffuse.reset();
	m_map.Destroy();
}
//-----------------------------------------------------------------------------
void World::Update(float deltaTime)
{
	m_map.Update(deltaTime);
}
//-----------------------------------------------------------------------------
void World::Draw(const Viewport& view)
{
	auto& renderSystem = GetRenderSystem();


	renderSystem.Bind(spTile);
	renderSystem.Bind(diffuse, 0);
	renderSystem.SetUniform("uMaterial.diffuse", 0);
	renderSystem.SetUniform("uMaterial.specular", glm::vec3(0.7f, 0.7f, 0.7f));
	renderSystem.SetUniform("uMaterial.shininess", 32.0f);

	renderSystem.SetUniform("uLight.direction", glm::vec3(-0.2f, -1.0f, 0.3f));
	renderSystem.SetUniform("uLight.ambient", glm::vec3(0.2f, 0.2f, 0.2f));
	renderSystem.SetUniform("uLight.diffuse", glm::vec3(1.0f, 1.0f, 1.0f));
	renderSystem.SetUniform("uLight.specular", glm::vec3(1.0f, 1.0f, 1.0f));

	renderSystem.SetUniform("uViewPosition", view.camera.position);
	renderSystem.SetUniform("uWorld", glm::mat4(1.0f));
	renderSystem.SetUniform("uView", view.camera.GetViewMatrix());
	renderSystem.SetUniform("uProjection", view.perspective);

	//renderSystem.Draw(geom);

	for (size_t x = 0; x < 100; x++)
	{
		for (size_t y = 0; y < 100; y++)
		{
			renderSystem.SetUniform("uWorld", glm::translate(glm::mat4(1.0f), glm::vec3(x, 0.0f, y)));
			GetGraphicsSystem().Draw(model);
		}
	}

	m_map.Draw(view);
}
//-----------------------------------------------------------------------------