#pragma once
#include "SDK.h"
#include "Player Manager.h"

#define M_PI 3.14159265359

class CCreateMove
{
public:
	void Invoke( CUserCmd* pCommand );
	void Triggerbot(CUserCmd* cmd, CSetupPlayer pMe);
private:
	void __stdcall CL_FixMove(CUserCmd* cmd, Vector viewangles)
	{
		Vector move, movenorm, dir, set;
		Vector movenormang, adjusted;

		float len;

		move.Init(cmd->forwardmove, cmd->sidemove, cmd->upmove);

		movenorm = move;

		len = move.Length();

		VectorAngles(movenorm, movenormang);

		if ((cmd->viewangles.x >= 89.0f) || (cmd->viewangles.x <= -89.0f))
		{
			if (viewangles.x >= 0.0f && viewangles.x <= 89.0f)
			{
				viewangles.x = cmd->viewangles.x + 180.0f;
			}
			if (viewangles.x <= 0.0f && viewangles.x >= -89.0f)
			{
				viewangles.x = cmd->viewangles.x - 180.0f;
			}
		}

		adjusted = movenormang + (cmd->viewangles - viewangles);

		AngleVectors(adjusted, &dir);

		set = dir * len;

		if ((cmd->viewangles.x > 89.0f) || (cmd->viewangles.x < -89.0f))
			cmd->forwardmove = set.x;
		else if ((cmd->viewangles.x == 89.0f || cmd->viewangles.x == -89.0f))
			cmd->forwardmove = -set.x;
		else
			cmd->forwardmove = set.x;

		if ((cmd->viewangles.x >= 89.0f) || (cmd->viewangles.x <= -89.0f))
			cmd->sidemove = -set.y;
		else
			cmd->sidemove = set.y;
	}

	void inline SinCos(float radians, float *sine, float *cosine)
	{
		*sine = sin(radians);
		*cosine = cos(radians);


	}

	void VectorAngles(const Vector& forward, Vector &angles)
	{
		Assert(s_bMathlibInitialized);
		float	tmp, yaw, pitch;

		if (forward[1] == 0 && forward[0] == 0)
		{
			yaw = 0;
			if (forward[2] > 0)
				pitch = 270;
			else
				pitch = 90;
		}
		else
		{
			yaw = (atan2(forward[1], forward[0]) * 180 / M_PI);
			if (yaw < 0)
				yaw += 360;

			tmp = sqrt(forward[0] * forward[0] + forward[1] * forward[1]);
			pitch = (atan2(-forward[2], tmp) * 180 / M_PI);
			if (pitch < 0)
				pitch += 360;
		}

		angles[0] = pitch;
		angles[1] = yaw;
		angles[2] = 0;
	}
	enum
	{
		PITCH = 0,	// up / down
		YAW,		// left / right
		ROLL		// fall over
	};

	void AngleVectors(const Vector &angles, Vector *forward)
	{
		Assert(s_bMathlibInitialized);
		Assert(forward);

		float	sp, sy, cp, cy;

		SinCos(DEG2RAD(angles[YAW]), &sy, &cy);
		SinCos(DEG2RAD(angles[PITCH]), &sp, &cp);

		forward->x = cp*cy;
		forward->y = cp*sy;
		forward->z = -sp;
	}
	void AngleVectors(const Vector &angles, Vector *forward, Vector *right, Vector *up)
	{
		Assert(s_bMathlibInitialized);

		float sr, sp, sy, cr, cp, cy;

#ifdef _X360
		fltx4 radians, scale, sine, cosine;
		radians = LoadUnaligned3SIMD(angles.Base());
		scale = ReplicateX4(M_PI_F / 180.f);
		radians = MulSIMD(radians, scale);
		SinCos3SIMD(sine, cosine, radians);
		sp = SubFloat(sine, 0);	sy = SubFloat(sine, 1);	sr = SubFloat(sine, 2);
		cp = SubFloat(cosine, 0);	cy = SubFloat(cosine, 1);	cr = SubFloat(cosine, 2);
#else
		SinCos(DEG2RAD(angles[YAW]), &sy, &cy);
		SinCos(DEG2RAD(angles[PITCH]), &sp, &cp);
		SinCos(DEG2RAD(angles[ROLL]), &sr, &cr);
#endif

		if (forward)
		{
			forward->x = cp*cy;
			forward->y = cp*sy;
			forward->z = -sp;
		}

		if (right)
		{
			right->x = (-1 * sr*sp*cy + -1 * cr*-sy);
			right->y = (-1 * sr*sp*sy + -1 * cr*cy);
			right->z = -1 * sr*cp;
		}

		if (up)
		{
			up->x = (cr*sp*cy + -sr*-sy);
			up->y = (cr*sp*sy + -sr*cy);
			up->z = cr*cp;
		}
	}
	bool __stdcall GetVisible(Vector& start, Vector& end, CBaseEntity* target)
	{
		player_info_t info;
		trace_t trace;
		Ray_t ray;
		ray.Init(start, end);
		g_pTrace->TraceRay(ray, 0x46004003, NULL, &trace);

		if (trace.flFraction == 1.f)
			return true;

		if (trace.m_pEnt && target)
		{
			if (trace.m_pEnt->GetIndex() == 0 || trace.allSolid)
				return false;
			if ((g_pEngine->GetPlayerInfo(trace.m_pEnt->GetIndex(), &info) || target->GetIndex() == trace.m_pEnt->GetIndex()) && trace.flFraction > .97f)
				return true;
		}
		return false;
	}
	bool __stdcall GetHitboxPosition(int Hitbox, Vector& HitboxPos, CBaseEntity* target)
	{
		if (!target || target->IsDormant())
			return false;
		matrix3x4 matrix[128];
		if (!target->SetupBones(matrix, 128, 0x100, 0))
			return false;
		HitboxPos = Vector(matrix[Hitbox][0][3], matrix[Hitbox][1][3], matrix[Hitbox][2][3]);
		return true;
	}
};
extern CCreateMove gCreateMove;
extern Vector hitpos;