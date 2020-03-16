#include <Core/Entity.h>

#include <catch.hpp>
#include <gmock.h>

namespace Tests
{
	SCENARIO("TestAddChild", "Entity")
	{
		GIVEN("An entity with no child")
		{
			auto entity = trs::makePtrOwner<trc::Entity>();
			auto childOwner = trs::makePtrOwner<trc::Entity>();
			WHEN("Adding a child")
			{
				auto childShared = trs::SharedPtr(childOwner);
				entity->addChild(childShared);
				THEN("The child is added")
				{
					const auto& children = entity->getChildren();
					REQUIRE(children.size() == 1);
					REQUIRE(children[0] == childShared);
				}
				AND_THEN("The child's parent is set")
				{
					REQUIRE(childShared->getParent().getPtr() == entity.getPtr());
				}
				entity->removeChild(childShared);
			}
		}
	}

	SCENARIO("TestRemoveChild", "Entity")
	{
		GIVEN("An entity with one child")
		{
			auto entity = trs::makePtrOwner<trc::Entity>();
			auto childOwner = trs::makePtrOwner<trc::Entity>();
			auto childShared = trs::SharedPtr(childOwner);
			entity->addChild(childShared);
			WHEN("Removing the child")
			{
				entity->removeChild(childShared);
				THEN("The child is removed")
				{
					const auto& children = entity->getChildren();
					REQUIRE(children.empty());
				}
			}
		}
		GIVEN("An entity with many children")
		{
			auto entity = trs::makePtrOwner<trc::Entity>();
			auto childOwner1 = trs::makePtrOwner<trc::Entity>();
			auto childOwner2 = trs::makePtrOwner<trc::Entity>();
			auto childOwner3 = trs::makePtrOwner<trc::Entity>();

			auto childShared1 = trs::SharedPtr(childOwner1);
			auto childShared2 = trs::SharedPtr(childOwner2);
			auto childShared3 = trs::SharedPtr(childOwner3);
			entity->addChild(childShared1);
			entity->addChild(childShared2);
			entity->addChild(childShared3);
			WHEN("Removing the first child")
			{
				entity->removeChild(childShared1);
				THEN("The child is removed")
				{
					const auto& children = entity->getChildren();
					REQUIRE(children.size() == 2);
					REQUIRE(children[0] == childShared2);
					REQUIRE(children[1] == childShared3);
				}
			}
			WHEN("Removing the middle child")
			{
				entity->removeChild(childShared2);
				THEN("The child is removed")
				{
					const auto& children = entity->getChildren();
					REQUIRE(children.size() == 2);
					REQUIRE(children[0] == childShared1);
					REQUIRE(children[1] == childShared3);
				}
			}
			WHEN("Removing the last child")
			{
				entity->removeChild(childShared3);
				THEN("The child is removed")
				{
					const auto& children = entity->getChildren();
					REQUIRE(children.size() == 2);
					REQUIRE(children[0] == childShared1);
					REQUIRE(children[1] == childShared2);
				}
			}
		}
	}

	SCENARIO("SetParent", "Entity")
	{
		GIVEN("An entity")
		{
			auto parentOwner = trs::makePtrOwner<trc::Entity>();
			auto entity = trs::makePtrOwner<trc::Entity>();

			auto parentShared1 = trs::SharedPtr(parentOwner);
			WHEN("When setting it's parent")
			{
				entity->setParent(parentShared1);
				THEN("The parent is set")
				{
					REQUIRE(entity->getParent() == parentShared1);
				}
				AND_THEN("The parent's child is added")
				{
					const auto& children = parentShared1->getChildren();
					REQUIRE(children.size() == 1);
					REQUIRE(children[0].getPtr() == entity.getPtr());
				}
			}
		}
	}
}  // namespace Tests