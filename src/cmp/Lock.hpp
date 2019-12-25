#pragma once

struct Lock {
	Lock() = default;

	const int ID { nextID++ };

	private:
		static int nextID;
};