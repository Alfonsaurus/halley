/*****************************************************************\
           __
          / /
		 / /                     __  __
		/ /______    _______    / / / / ________   __       __
	   / ______  \  /_____  \  / / / / / _____  | / /      / /
	  / /      | / _______| / / / / / / /____/ / / /      / /
	 / /      / / / _____  / / / / / / _______/ / /      / /
	/ /      / / / /____/ / / / / / / |______  / |______/ /
   /_/      /_/ |________/ / / / /  \_______/  \_______  /
                          /_/ /_/                     / /
			                                         / /
		       High Level Game Framework            /_/

  ---------------------------------------------------------------

  Copyright (c) 2007-2011 - Rodrigo Braz Monteiro.
  This file is subject to the terms of halley_license.txt.

\*****************************************************************/

#include "halley/core/graphics/camera.h"
#include "graphics/render_target/render_target.h"

using namespace Halley;


Camera::Camera()
	: zoom(1.0f)
{
}


Camera::Camera(Vector2f _pos, Angle1f _angle)
	: pos(_pos)
	, angle(_angle)
	, zoom(1.0f)
{
}


Camera& Camera::setPosition(Vector2f _pos)
{
	pos = _pos;
	return *this;
}


Camera& Camera::setAngle(Angle1f _angle)
{
	angle = _angle;
	return *this;
}


Camera& Camera::setZoom(float _zoom)
{
	zoom = _zoom;
	return *this;
}

Camera& Camera::resetRenderTarget()
{
	renderTarget = nullptr;
	return *this;
}

Camera& Camera::setRenderTarget(RenderTarget& target)
{
	renderTarget = &target;
	return *this;
}

Camera& Camera::resetViewPort()
{
	viewPort.reset();
	return *this;
}

Camera& Camera::setViewPort(Rect4i v)
{
	viewPort = v;
	return *this;
}

void Camera::updateProjection(bool flipVertical)
{
	Vector2i area = getActiveViewPort().getSize();

	// Setup projection
	const float w = float(area.x);
	const float h = float(area.y);
	projection = Matrix4f::makeOrtho2D(-w/2, w/2, flipVertical ? h/2 : -h/2, flipVertical ? -h/2 : h/2, -1000, 1000);

	// Camera properties
	if (zoom != 1.0f) {
		projection.scale(Vector2f(zoom, zoom));
	}
	if (angle.getRadians() != 0) {
		projection.rotateZ(-angle);
	}
	if (pos != Vector2f()) {
		projection.translate(-pos);
	}
}

RenderTarget& Camera::getActiveRenderTarget() const
{
	Expects(rendering);
	return renderTarget ? *renderTarget : *defaultRenderTarget;
}

RenderTarget* Camera::getRenderTarget() const
{
	return renderTarget;
}

Rect4i Camera::getActiveViewPort() const
{
	auto targetViewPort = getActiveRenderTarget().getViewPort();
	if (viewPort) {
		return viewPort.get().intersection(targetViewPort);
	} else {
		return targetViewPort;
	}
}

Rect4f Camera::getClippingRectangle() const
{
	auto vp = getActiveViewPort();
	auto halfSize = Vector2f(vp.getSize()) / (zoom * 2);
	auto a = halfSize.rotate(angle);
	auto b = Vector2f(-halfSize.x, halfSize.y).rotate(angle);
	auto rotatedHalfSize = Vector2f(std::max(std::abs(a.x), std::abs(b.x)), std::max(std::abs(a.y), std::abs(b.y)));
	return Rect4f(pos - rotatedHalfSize, pos + rotatedHalfSize);
}

Vector2f Camera::screenToWorld(Vector2f p, Rect4f viewport) const
{
	return ((p - viewport.getCenter()) / zoom).rotate(angle) + pos;
}

Vector2f Camera::worldToScreen(Vector2f p, Rect4f viewport) const
{
	return (p - pos).rotate(-angle) * zoom + viewport.getCenter();
}
