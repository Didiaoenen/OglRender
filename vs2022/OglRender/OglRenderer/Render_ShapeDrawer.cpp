
#include "Render_ShaderLoader.h"
#include "Render_ShapeDrawer.h"

Render::Render_ShapeDrawer::Render_ShapeDrawer(Render_Renderer& pRenderer) :
	mRenderer(pRenderer)
{
	std::vector<Render_Vertex> vertices;
	vertices.push_back
	({
		0, 0, 0,
		0, 0,
		0, 0, 0,
		0, 0, 0,
		0, 0, 0
		});
	vertices.push_back
	({
		0, 0, 0,
		0, 0,
		0, 0, 0,
		0, 0, 0,
		0, 0, 0
		});

	mLineMesh = new Render_Mesh(vertices, { 0, 1 }, 0);

	std::string vertexShader = R"(
#version 430 core

uniform vec3 start;
uniform vec3 end;
uniform mat4 viewProjection;

void main()
{
	vec3 position = gl_VertexID == 0 ? start : end;
    gl_Position = viewProjection * vec4(position, 1.0);
}

)";

	std::string fragmentShader = R"(
#version 430 core

uniform vec3 color;

out vec4 FRAGMENT_COLOR;

void main()
{
	FRAGMENT_COLOR = vec4(color, 1.0);
}
)";

	mLineShader = Render_ShaderLoader::CreateFromSource(vertexShader, fragmentShader);

	vertexShader = R"(
#version 430 core

uniform vec3 start;
uniform vec3 end;
uniform mat4 viewProjection;

out vec3 fragPos;

void main()
{
	vec3 position = gl_VertexID == 0 ? start : end;
	fragPos = position;
    gl_Position = viewProjection * vec4(position, 1.0);
}

)";

	fragmentShader = R"(
#version 430 core

uniform vec3 color;
uniform vec3 viewPos;
uniform float linear;
uniform float quadratic;
uniform float fadeThreshold;

out vec4 FRAGMENT_COLOR;

in vec3 fragPos;

float AlphaFromAttenuation()
{
	vec3 fakeViewPos = viewPos;
	fakeViewPos.y = 0;

    const float distanceToLight = max(max(length(viewPos - fragPos) - fadeThreshold, 0) - viewPos.y, 0);
    const float attenuation = (linear * distanceToLight + quadratic * (distanceToLight * distanceToLight));
    return 1.0 / attenuation;
}

void main()
{
	FRAGMENT_COLOR = vec4(color, AlphaFromAttenuation());
}
)";

	mGridShader = Render_ShaderLoader::CreateFromSource(vertexShader, fragmentShader);
}

Render::Render_ShapeDrawer::~Render_ShapeDrawer()
{
	delete mLineMesh;
	Render_ShaderLoader::Destroy(mLineShader);
}

void Render::Render_ShapeDrawer::SetViewProjection(const glm::mat4& pViewProjection)
{
	if (auto it = mLineShader->mPrograms.find("universal"); it != mLineShader->mPrograms.end())
	{
		auto& [_, program] = *it;

		program->Bind();
		program->SetUniformMat4("viewProjection", pViewProjection);
		program->Unbind();
	}

	if (auto it = mGridShader->mPrograms.find("universal"); it != mGridShader->mPrograms.end())
	{
		auto& [_, program] = *it;

		program->Bind();
		program->SetUniformMat4("viewProjection", pViewProjection);
		program->Unbind();
	}
}

void Render::Render_ShapeDrawer::DrawLine(const glm::vec3& pStart, const glm::vec3& pEnd, const glm::vec3& pColor, float pLineWidth)
{
	if (auto it = mLineShader->mPrograms.find("universal"); it != mLineShader->mPrograms.end())
	{
		auto& [_, program] = *it;

		program->Bind();

		program->SetUniformVec3("start", pStart);
		program->SetUniformVec3("end", pEnd);
		program->SetUniformVec3("color", pColor);

		mRenderer.SetRasterizationMode(ERasterizationMode::LINE);
		mRenderer.SetRasterizationLinesWidth(pLineWidth);
		mRenderer.Draw(*mLineMesh, EPrimitiveMode::LINES);
		mRenderer.SetRasterizationLinesWidth(1.0f);
		mRenderer.SetRasterizationMode(ERasterizationMode::FILL);

		program->Unbind();
	}
}

void Render::Render_ShapeDrawer::DrawGrid(const glm::vec3& pViewPos, const glm::vec3& pColor, int32_t pGridSize, float pLinear, float pQuadratic, float pFadeThreshold, float pLineWidth)
{
	if (auto it = mGridShader->mPrograms.find("universal"); it != mGridShader->mPrograms.end())
	{
		auto& [_, program] = *it;

		program->Bind();
		program->SetUniformVec3("color", pColor);
		program->SetUniformVec3("viewPos", pViewPos);
		program->SetUniformFloat("linear", pLinear);
		program->SetUniformFloat("quadratic", pQuadratic);
		program->SetUniformFloat("fadeThreshold", pFadeThreshold);

		mRenderer.SetRasterizationMode(ERasterizationMode::LINE);
		mRenderer.SetRasterizationLinesWidth(pLineWidth);
		mRenderer.SetCapability(ERenderingCapability::BLEND, true);

		for (int32_t i = -pGridSize + 1; i < pGridSize; ++i)
		{
			program->SetUniformVec3("start", { -(float)pGridSize + std::floor(pViewPos.x), 0.f, (float)i + std::floor(pViewPos.z) });
			program->SetUniformVec3("end", { (float)pGridSize + std::floor(pViewPos.x), 0.f, (float)i + std::floor(pViewPos.z) });
			mRenderer.Draw(*mLineMesh, EPrimitiveMode::LINES);

			program->SetUniformVec3("start", { (float)i + std::floor(pViewPos.x), 0.f, -(float)pGridSize + std::floor(pViewPos.z) });
			program->SetUniformVec3("end", { (float)i + std::floor(pViewPos.x), 0.f, (float)pGridSize + std::floor(pViewPos.z) });
			mRenderer.Draw(*mLineMesh, EPrimitiveMode::LINES);
		}

		mRenderer.SetCapability(ERenderingCapability::BLEND, false);
		mRenderer.SetRasterizationLinesWidth(1.0f);
		mRenderer.SetRasterizationMode(ERasterizationMode::FILL);
		program->Unbind();
	}
}
