#pragma once

struct IVideo {
	virtual ~IVideo() = default;

	virtual void render() const = 0;

	virtual void reset() = 0;

	virtual void nextFrame() = 0;

	virtual double getCurrentFrame() = 0;

	virtual double getNumFrames() = 0;

	virtual double getFPS() = 0;

	virtual void setLoop(bool) = 0;
};