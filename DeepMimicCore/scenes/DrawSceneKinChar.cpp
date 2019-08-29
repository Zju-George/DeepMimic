#include "DrawSceneKinChar.h"
#include "render/DrawUtil.h"
#include "render/DrawCharacter.h"
#include "render/DrawWorld.h"
#include "util/FileUtil.h"

const double gLinkWidth = 0.025f;
const tVector gLineColor = tVector(0, 0, 0, 1);
const tVector gFilLColor = tVector(0.6f, 0.65f, 0.675f, 1);
const tVector gCamFocus0 = tVector(0, 0.75, 0, 0);
const double gGroundHeight = 0;

cDrawSceneKinChar::cDrawSceneKinChar() : cDrawScene()
{
}

cDrawSceneKinChar::~cDrawSceneKinChar()
{
}

void cDrawSceneKinChar::Init()
{
	BuildScene(mScene);
	SetupScene(mScene);

	cDrawScene::Init();
}

void cDrawSceneKinChar::Reset()
{
	cDrawScene::Reset();
	mScene->Reset();
}

void cDrawSceneKinChar::Clear()
{
	cDrawScene::Clear();
	mScene->Clear();
}

void cDrawSceneKinChar::Update(double time_elapsed)
{
	cDrawScene::Update(time_elapsed);
	UpdateScene(time_elapsed);
	UpdateCamera();
}

std::string cDrawSceneKinChar::GetName() const
{
	return mScene->GetName();
}

void cDrawSceneKinChar::BuildScene(std::shared_ptr<cSceneKinChar>& out_scene) const
{
	out_scene = std::shared_ptr<cSceneKinChar>(new cSceneKinChar());
}

void cDrawSceneKinChar::SetupScene(std::shared_ptr<cSceneKinChar>& out_scene)
{
	out_scene->ParseArgs(mArgParser);
	out_scene->Init();
}

void cDrawSceneKinChar::UpdateScene(double time_elapsed)
{
	mScene->Update(time_elapsed);
}

tVector cDrawSceneKinChar::GetCamTrackPos() const
{
	return mScene->GetCharPos();
}

tVector cDrawSceneKinChar::GetCamStillPos() const
{
	return mScene->GetCharPos();
}

tVector cDrawSceneKinChar::GetDefaultCamFocus() const
{
	return gCamFocus0;
}
void cDrawSceneKinChar::DrawAxis() const
{
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
void cDrawSceneKinChar::DrawMisc() const
{
	DrawAxis();
}
void cDrawSceneKinChar::DrawGround() const
{
	tVector ground_col = GetGroundColor();
	cDrawUtil::SetColor(ground_col);
	DrawGround3D();
}

void cDrawSceneKinChar::DrawGround3D() const
{
	const double w = 200;
	const tVector ground_origin = tVector(0, gGroundHeight, 0, 0);
	const tVector tex_size = tVector(0.5, 0.5, 0, 0);

	const auto& character = mScene->GetCharacter();
	tVector char_pos = character->GetRootPos();
	char_pos[1] = gGroundHeight;
	tVector a = char_pos - tVector(-0.5 * w, 0, -0.5 * w, 0);
	tVector b = char_pos - tVector(-0.5 * w, 0, 0.5 * w, 0);
	tVector c = char_pos - tVector(0.5 * w, 0, 0.5 * w, 0);
	tVector d = char_pos - tVector(0.5 * w, 0, -0.5 * w, 0);

	tVector min_coord = a - ground_origin;
	tVector max_coord = c - ground_origin;
	min_coord[0] /= tex_size[0];
	min_coord[1] = min_coord[2] / tex_size[1];
	max_coord[0] /= tex_size[0];
	max_coord[1] = max_coord[2] / tex_size[1];

	tVector coord_a = tVector(min_coord[0], min_coord[1], 0, 0);
	tVector coord_b = tVector(min_coord[0], max_coord[1], 0, 0);
	tVector coord_c = tVector(max_coord[0], max_coord[1], 0, 0);
	tVector coord_d = tVector(max_coord[0], min_coord[1], 0, 0);

	cDrawUtil::DrawQuad(a, b, c, d, coord_a, coord_b, coord_c, coord_d);
}


void cDrawSceneKinChar::DrawCharacters() const
{
	const auto& character = mScene->GetCharacter();
	cDrawCharacter::Draw(*character, gLinkWidth, gFilLColor, gLineColor);
}