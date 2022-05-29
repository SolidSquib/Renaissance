#include "Renaissance/Core/Core.h"

#include "Renaissance/Graphics/Buffer.h"
#include "Renaissance/Graphics/Renderer.h"
#include "Renaissance/Graphics/SubTexture.h"
#include "Renaissance/Scene/Component.h"

namespace Renaissance::Graphics
{
	using namespace Math;

	class Shader;
	class Texture2D;
	class VertexArray;

	class SpriteBatch
	{
	public:
		struct VertexData
		{
			Vector3 Position;
			Vector4 Color;
			Vector2 TexCoord;
			uint32_t TexIndex = 0;
		};

		SpriteBatch();
		SpriteBatch(const SharedPtr<Shader>& shader);
		~SpriteBatch();
		
		static void Init();
		static void Shutdown();

		void Draw(const Vector2& location, const Vector2& size, const Vector4& tint);
		void Draw(const Vector3& location, const Vector2& size, const Vector4& tint);
		void Draw(const Matrix4& transform, const Vector2& size, const Vector4& tint);

		void Draw(const Vector3& location, const Vector2& size, const SharedPtr<SubTexture2D>& texture, const Vector2& tilingFactor, const Vector4& tint = Vector4(1.0f));
		void Draw(const Vector2& location, const Vector2& size, const SharedPtr<SubTexture2D>& texture, const Vector2& tilingFactor, const Vector4& tint = Vector4(1.0f));
		void Draw(const Matrix4& transform, const Vector2& size, const SharedPtr<SubTexture2D>& texture, const Vector2& tilingFactor, const Vector4& tint = Vector4(1.0f));

		void Draw(const Vector2& location, const Vector2& size, const SharedPtr<Texture2D>& texture, const Vector2& textureCoordMin, const Vector2& textureCoordMax, const Vector2& tilingFactor, const Vector4& tint = Vector4(1.0f));
		void Draw(const Vector3& location, const Vector2& size, const SharedPtr<Texture2D>& texture, const Vector2& textureCoordMin, const Vector2& textureCoordMax, const Vector2& tilingFactor, const Vector4& tint = Vector4(1.0f));
		void Draw(const Matrix4& transform, const Vector2& size, const SharedPtr<Texture2D>& texture, const Vector2& textureCoordMin, const Vector2& textureCoordMax, const Vector2& tilingFactor, const Vector4& tint = Vector4(1.0f));

		void Draw(const Vector2& location, const SpriteRendererComponent& spriteComponent);
		void Draw(const Vector3& location, const SpriteRendererComponent& spriteComponent);
		void Draw(const Matrix4& transform, const SpriteRendererComponent& spriteComponent);

		void End();

	private:
		void Flush();

	public:
		static const uint32_t MaxQuads = 20000;
		static const uint32_t MaxVertices = MaxQuads * 4;
		static const uint32_t MaxIndicies = MaxQuads * 6;
		static const uint32_t MaxTextureUnits = 16;
		static const Vector4 QuadBaseVertices[];

	private:
		SharedPtr<Shader> mShader;		
		uint32_t mVertexCount = 0;
		uint32_t mIndexCount = 0;
		uint32_t mTextureUnitCount = 1;
		bool mActive = true;

		static SharedPtr<VertexArray> SharedVertexArray;
		static SharedPtr<VertexBuffer> SharedVertexBuffer;
		static SharedPtr<IndexBuffer> SharedIndexBuffer;
		static SharedPtr<Shader> DefaultShader;
		static SharedPtr<Texture2D> WhiteTexture;
		static SharedPtr<Texture2D>* SharedTextureUnitPtr;
		static VertexData* SharedVertexPtr;

		static uint32_t ActiveSpriteBatchCount;
	};
}