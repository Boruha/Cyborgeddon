#pragma once

struct Lock : public Component {
	explicit Lock(const EntityType& e_type, const std::size_t& e_ID) : Component(e_type, e_ID) {  }

	[[nodiscard]] int getLockID() const { return ID; }

	private:
		const int ID { nextID++ };
		inline static int nextID { 0 };
};