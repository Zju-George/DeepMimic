#include "DrawWorld.h"
#include "render/DrawPerturb.h"
#include "render/DrawUtil.h"
void cDrawWorld::DrawPerturbs(const cWorld& world)
{
	const cPerturbManager& perturb_man = world.GetPerturbManager();
	int num_perturbs = perturb_man.GetNumPerturbs();
	for (int p = 0; p < num_perturbs; ++p)
	{
		const tPerturb& perturb = perturb_man.GetPerturb(p);
		cDrawPerturb::Draw(perturb);
	}
}

void cDrawWorld::DrawAxis(const cWorld & world)
{
	// george draw Axis
	// X axis
	cDrawUtil::SetColor(tVector(1, 0, 0, 0.5));
	tVector start = tVector(0, 0.01, 0, 1);
	tVector end = tVector(10, 0.01, 0, 1);
	cDrawUtil::DrawLine(start, end);

	// Y axis
	cDrawUtil::SetColor(tVector(0, 1, 0, 0.5));
	start = tVector(0, 0.01, 0, 1);
	end = tVector(0, 10, 0, 1);
	cDrawUtil::DrawLine(start, end);

	// Z axis
	cDrawUtil::SetColor(tVector(0, 0, 1, 0.5));
	start = tVector(0, 0.01, 0, 1);
	end = tVector(0, 0.01, 10, 1);
	cDrawUtil::DrawLine(start, end);
}
