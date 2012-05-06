#include "Velocity3D.h"

namespace cole {
namespace math {

const Velocity3D Velocity3D::INVALID_V3D(Velocity(-999.0, Velocity::MPS),
		Velocity(-999.0, Velocity::MPS), Velocity(-999.0, Velocity::MPS));

Velocity3D::Velocity3D() :
	_vnorth(), _veast(), _vdown() {
	;
}
Velocity3D::Velocity3D(const Velocity &vnorth, const Velocity &veast,
		const Velocity &vdown) {
	_vnorth = vnorth;
	_veast = veast;
	_vdown = vdown;
}
Velocity3D::Velocity3D(const Velocity3D &v3d) {
	_vnorth = v3d._vnorth;
	_veast = v3d._veast;
	_vdown = v3d._vdown;
}
Velocity3D::~Velocity3D() {
	;
}

Velocity Velocity3D::getVNorth() const {
	return _vnorth;
}
Velocity Velocity3D::getVEast() const {
	return _veast;
}
Velocity Velocity3D::getVDown() const {
	return _vdown;
}

string Velocity3D::toString() {
	string ret("Velocity3D: <");
	ret += _vnorth.toString() + " ";
	ret += _veast.toString() + " ";
	ret += _vdown.toString() + ">";
	return ret;
}

bool Velocity3D::operator==(const Velocity3D& other) const {
	bool ans = (_vnorth == other._vnorth);
	ans = ans && (_veast == other._veast);
	ans = ans && (_vdown == other._vdown);
	return ans;
}

bool Velocity3D::operator!=(const Velocity3D& other) const {
	return !operator==(other);
}

} // namespace math
} // namespace cole
