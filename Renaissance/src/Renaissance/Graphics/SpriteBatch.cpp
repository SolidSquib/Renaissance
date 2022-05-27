#include "RenaissancePCH.h"
#include "Renaissance/Graphics/Camera.h"
#include "Renaissance/Graphics/SpriteBatch.h"
#include "Renaissance/Graphics/RenderCommands.h"
#include "Renaissance/Graphics/Shader.h"
#include "Renaissance/Graphics/Texture.h"
#include "Renaissance/Graphics/VertexArray.h"
#include "Renaissance/Graphics/Renderer.h"

namespace Renaissance::Graphics
{
	const Vector4 SpriteBatch::QuadBaseVertices[] = { { -0.5f, -0.5f, 0.0f, 1.0f }, {  0.5f, -0.5f, 0.0f, 1.0f }, {  0.5f,  0.5f, 0.0f, 1.0f }, { -0.5f,  0.5f, 0.0f, 1.0f } };

	SharedPtr<VertexArray> SpriteBatch::SharedVertexArray = nullptr;
	SharedPtr<VertexBuffer> SpriteBatch::SharedVertexBuffer = nullptr;
	SharedPtr<IndexBuffer> SpriteBatch::SharedIndexBuffer = nullptr;
	SharedPtr<Shader> SpriteBatch::DefaultShader = nullptr;
	SharedPtr<Texture2D> SpriteBatch::WhiteTexture = nullptr;
	SharedPtr<Texture2D>* SpriteBatch::SharedTextureUnitPtr = nullptr;
	SpriteBatch::VertexData* SpriteBatch::SharedVertexPtr = nullptr;
	uint32_t SpriteBatch::ActiveSpriteBatchCount = 0;

	SpriteBatch::SpriteBatch()
		: SpriteBatch(DefaultShader)
	{ }

	SpriteBatch::SpriteBatch(const SharedPtr<Shader>& shader)
		: mShader(shader)
	{
		REN_CORE_ASSERT(ActiveSpriteBatchCount == 0, "SpriteBatch already active!");

		if (!shader)
		{
			REN_CORE_WARN("SpriteBatch passed an invalid shader object. Create SpriteBatch with no parameters to use default shader.");
			mShader = DefaultShader;
		}

		ActiveSpriteBatchCount++;
	}

	SpriteBatch::~SpriteBatch()
	{
		End();
	}

	void SpriteBatch::Init()
	{
		REN_CORE_ASSERT(SharedTextureUnitPtr == nullptr && SharedVertexPtr == nullptr);
		
		uint32_t white = 0xffffffff;
		WhiteTexture = Texture2D::Create(1, 1, &white);

		SharedVertexPtr = new VertexData[MaxVertices];
		SharedTextureUnitPtr = new SharedPtr<Texture2D>[MaxTextureUnits];
		SharedTextureUnitPtr[0] = WhiteTexture;


		SharedVertexArray = VertexArray::Create();

		SharedVertexBuffer = VertexBuffer::Create(MaxVertices * sizeof(VertexData));
		BufferLayout vertexLayout = {
				{ ShaderDataType::Float3, "a_Position" },
				{ ShaderDataType::Float4, "a_Color" },
				{ ShaderDataType::Float2, "a_TexCoord" },
				{ ShaderDataType::Uint, "a_TexIndex" }
		};
		SharedVertexBuffer->SetLayout(vertexLayout);

		uint32_t offset = 0;
		uint32_t* indices = new uint32_t[MaxIndicies];
		for (uint32_t i = 0; i < MaxIndicies; i += 6)
		{
			indices[i + 0] = offset + 0;
			indices[i + 1] = offset + 1;
			indices[i + 2] = offset + 2;

			indices[i + 3] = offset + 2;
			indices[i + 4] = offset + 3;
			indices[i + 5] = offset + 0;

			offset += 4;
		}
		SharedIndexBuffer = IndexBuffer::Create(indices, MaxIndicies);
		delete[] indices;

		SharedVertexArray->AddVertexBuffer(SharedVertexBuffer);
		SharedVertexArray->SetIndexBuffer(SharedIndexBuffer);

		DefaultShader = ShaderLibrary::GetGlobal().Load("../Renaissance/assets/shaders/Texture.glsl");
	}

	void SpriteBatch::Shutdown()
	{
		if (SharedVertexPtr)
		{
			delete[] SharedVertexPtr;
			SharedVertexPtr = nullptr;
		}

		if (SharedTextureUnitPtr)
		{
			delete[] SharedTextureUnitPtr;
			SharedTextureUnitPtr = nullptr;
		}
	}
	
	void SpriteBatch::Draw(const Vector2& location, const Vector2& size, const Vector4& tint)
	{
		Matrix4 transform = glm::translate(IdentityMatrix, Vector3(location, 0.0f));
		Draw(transform, size, WhiteTexture, Vector2(0.0f), Vector2(1.0f), Vector2(1.0), tint);
	}

	void SpriteBatch::Draw(const Vector3& location, const Vector2& size, const Vector4& tint)
	{
		Matrix4 transform = glm::translate(IdentityMatrix, location);
		Draw(transform, size, WhiteTexture, Vector2(0.0f), Vector2(1.0f), Vector2(1.0), tint);
	}

	void SpriteBatch::Draw(const Matrix4& transform, const Vector2& size, const Vector4& tint)
	{
		Draw(transform, size, WhiteTexture, Vector2(0.0f), Vector2(1.0f), Vector2(1.0), tint);
	}

	void SpriteBatch::Draw(const Vector2& location, const Vector2& size, const SharedPtr<Texture2D>& texture, const Vector2& textureCoordMin, const Vector2& textureCoordMax, const Vector2& tilingFactor)
	{
		Matrix4 transform = glm::translate(IdentityMatrix, Vector3(location, 0.0f));
		Draw(transform, size, texture, textureCoordMin, textureCoordMax, tilingFactor, Vector4(1.0f, 1.0f, 1.0f, 1.0f));
	}

	void SpriteBatch::Draw(const Vector3& location, const Vector2& size, const SharedPtr<Texture2D>& texture, const Vector2& textureCoordMin, const Vector2& textureCoordMax, const Vector2& tilingFactor)
	{
		Matrix4 transform = glm::translate(IdentityMatrix, location);
		Draw(transform, size, texture, textureCoordMin, textureCoordMax, tilingFactor, Vector4(1.0f, 1.0f, 1.0f, 1.0f));
	}

	void SpriteBatch::Draw(const Matrix4& transform, const Vector2& size, const SharedPtr<Texture2D>& texture, const Vector2& textureCoordMin, const Vector2& textureCoordMax, const Vector2& tilingFactor)
	{
		Draw(transform, size, texture, textureCoordMin, textureCoordMax, tilingFactor, Vector4(1.0f, 1.0f, 1.0f, 1.0f));
	}

	void SpriteBatch::Draw(const Vector2& location, const Vector2& size, const SharedPtr<Texture2D>& texture, const Vector2& textureCoordMin, const Vector2& textureCoordMax, const Vector2& tilingFactor, const Vector4& tint)
	{
		Matrix4 transform = glm::translate(IdentityMatrix, Vector3(location, 0.0f));
		Draw(transform, size, texture, textureCoordMin, textureCoordMax, tilingFactor, tint);
	}

	void SpriteBatch::Draw(const Vector3& location, const Vector2& size, const SharedPtr<Texture2D>& texture, const Vector2& textureCoordMin, const Vector2& textureCoordMax, const Vector2& tilingFactor, const Vector4& tint)
	{
		Matrix4 transform = glm::translate(IdentityMatrix, location);
		Draw(transform, size, texture, textureCoordMin, textureCoordMax, tilingFactor, tint);
	}

	void SpriteBatch::Draw(const Matrix4& transform, const Vector2& size, const SharedPtr<Texture2D>& texture, const Vector2& textureCoordMin, const Vector2& textureCoordMax, const Vector2& tilingFactor, const Vector4& tint)
	{
		SharedPtr<Texture2D> useTexture = texture ? texture : WhiteTexture;

		bool foundTexture = false;
		int32_t textureIndex = 0;
		for (uint32_t i = 0; i < mTextureUnitCount; ++i)
		{
			if (*SharedTextureUnitPtr[i] == *useTexture)
			{
				foundTexture = true;
				textureIndex = i;
				break;
			}
		}

		if (!foundTexture)
		{
			if (mTextureUnitCount >= MaxTextureUnits)
			{
				REN_CORE_INFO("SpriteBatch has reached the limit of allowed texture units and is flushing early in order to continue.");
				Flush();
			}

			textureIndex = mTextureUnitCount++;
			SharedTextureUnitPtr[textureIndex] = useTexture;
		}

		if (mIndexCount >= MaxIndicies)
		{
			REN_CORE_INFO("SpriteBatch has reached the limit of allowed indices in a single batch and will now flush early in order to continue.");
			Flush();
		}

		Vector2 textureCoords[] =
		{
			textureCoordMin,
			Vector2(textureCoordMax.x, textureCoordMin.y),
			textureCoordMax,
			Vector2(textureCoordMin.x, textureCoordMax.y)
		};

		Vector4 scale(size, 1.0f, 1.0f);
		constexpr uint32_t numVerticesPerQuad = 4;
		for (uint32_t i = 0; i < numVerticesPerQuad; ++i)
		{			 
			SharedVertexPtr[i + mVertexCount].Position = transform * (QuadBaseVertices[i] * scale);
			SharedVertexPtr[i + mVertexCount].Color = tint;
			SharedVertexPtr[i + mVertexCount].TexCoord = textureCoords[i];
			SharedVertexPtr[i + mVertexCount].TexIndex = textureIndex;
		}

		mVertexCount += numVerticesPerQuad;
		mIndexCount += 6;
	}

	/// <summary>
	/// Flushes the currently batched data and marks this sprite batch as finished.
	/// </summary>
	void SpriteBatch::End()
	{
		if (mActive)
		{
			Flush();
			mActive = false;
			ActiveSpriteBatchCount--;
		}
	}

	/// <summary>
	/// Passes the currently batched data onto the renderer.
	/// </summary>
	void SpriteBatch::Flush()
	{
		if (!mActive)
		{
			REN_CORE_WARN("Cannot flush inactive SpriteBatch.");
			return;
		}

		if (mIndexCount == 0)
		{
			// nothing to draw.
			REN_CORE_INFO("SpriteBatch flushed with 0 added elements.");
			return;
		}

		for (uint32_t i = 0; i < mTextureUnitCount; ++i)
		{
			SharedTextureUnitPtr[i]->Bind(i);
		}

		mShader->Bind();
		mShader->SetMatrix4("u_ViewProjection", Renderer::Get().GetCachedViewProjection());

		size_t dataSize = sizeof(VertexData) * mVertexCount;
		SharedVertexBuffer->SetData(SharedVertexPtr, dataSize, 0);

		RenderCommand::DrawIndexed(SharedVertexArray, mIndexCount);

		mVertexCount = 0;
		mIndexCount = 0;
		mTextureUnitCount = 1;
	}
}