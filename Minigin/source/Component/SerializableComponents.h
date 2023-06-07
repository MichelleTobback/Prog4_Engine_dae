#pragma once

#include "Scene/Serializable.h"

namespace dae
{
	class Component;

	//=================================
	// Quad
	//=================================
	class QuadSerializer final : public Serializable
	{
	public:
		QuadSerializer();
		virtual ~QuadSerializer() = default;

		virtual void Serialize(BinaryWriter& out, Component* pComponent) const override;
		virtual Component* Deserialize(DeserializeParams& params) override;

	private:
	};

	//=================================
	// textureRenderer
	//=================================
	class TextureRendererSerializer final : public Serializable
	{
	public:
		TextureRendererSerializer();
		virtual ~TextureRendererSerializer() override = default;

		virtual void Serialize(BinaryWriter& out, Component* pComponent) const override;
		virtual Component* Deserialize(DeserializeParams& params) override;

	private:
	};

	//=================================
	// SpriteRenderer
	//=================================
	class SpriteRendererSerializer final : public Serializable
	{
	public:
		SpriteRendererSerializer();
		virtual ~SpriteRendererSerializer() override = default;

		virtual void Serialize(BinaryWriter& out, Component* pComponent) const override;
		virtual Component* Deserialize(DeserializeParams& params) override;

	private:
	};

	//=================================
	// QuadRenderer
	//=================================
	class QuadRendererSerializer final : public Serializable
	{
	public:
		QuadRendererSerializer();
		virtual ~QuadRendererSerializer() override = default;

		virtual void Serialize(BinaryWriter& out, Component* pComponent) const override;
		virtual Component* Deserialize(DeserializeParams& params) override;

	private:
	};

	//=================================
	// Texture
	//=================================
	class TextureSerializer final : public Serializable
	{
	public:
		TextureSerializer();
		virtual ~TextureSerializer() override = default;

		virtual void Serialize(BinaryWriter& out, Component* pComponent) const override;
		virtual Component* Deserialize(DeserializeParams& params) override;

	private:
	};

	//=================================
	// TextComponent
	//=================================
	class TextComponentSerializer final : public Serializable
	{
	public:
		TextComponentSerializer();
		virtual ~TextComponentSerializer() override = default;

		virtual void Serialize(BinaryWriter& out, Component* pComponent) const override;
		virtual Component* Deserialize(DeserializeParams& params) override;

	private:
	};

	//=================================
	// SpriteComponent
	//=================================
	class SpriteComponentSerializer final : public Serializable
	{
	public:
		SpriteComponentSerializer();
		virtual ~SpriteComponentSerializer() override = default;

		virtual void Serialize(BinaryWriter& out, Component* pComponent) const override;
		virtual Component* Deserialize(DeserializeParams& params) override;

	private:
	};

	//=================================
	// SpriteAtlasComponent
	//=================================
	class SpriteAtlasComponentSerializer final : public Serializable
	{
	public:
		SpriteAtlasComponentSerializer();
		virtual ~SpriteAtlasComponentSerializer() override = default;

		virtual void Serialize(BinaryWriter& out, Component* pComponent) const override;
		virtual Component* Deserialize(DeserializeParams& params) override;

	private:
	};
}