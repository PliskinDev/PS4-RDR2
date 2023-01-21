#pragma once
//2much4u
struct Vector2
{
	float x, y;
};

struct Vector3
{
	float x, y, z;
};

struct Vector4
{
	float x, y, z, w;
};

struct NativeArg_s
{
	uint64_t* m_ReturnValue;
	uint32_t m_ArgCount;
	uint8_t m_padding1[4];
	uint64_t* m_ArgValues;
	uint32_t m_VecCount;
	uint8_t m_padding2[4];
	Vector3* m_ArgVec[4];
	Vector4 m_TempVec[4];
};

static NativeArg_s* g_NativeArg = (NativeArg_s*)0x846D7E0;

void SetVec() 
{
	while (g_NativeArg->m_VecCount)
	{
		g_NativeArg->m_VecCount--;
		Vector3* vec3 = g_NativeArg->m_ArgVec[g_NativeArg->m_VecCount];
		Vector4 vec4 = g_NativeArg->m_TempVec[g_NativeArg->m_VecCount];
		vec3->x = vec4.x;
		vec3->y = vec4.y;
		vec3->z = vec4.z;
	}
}

void ResetArgs() 
{
	g_NativeArg->m_ArgCount = 0;
	g_NativeArg->m_VecCount = 0;
	g_NativeArg->m_ArgValues = g_NativeArg->m_ReturnValue = (uint64_t*)0x846D800;
}

template<typename T>
inline void PushArg(T value) 
{
	*(T*)&g_NativeArg->m_ArgValues[g_NativeArg->m_ArgCount] = value;
	g_NativeArg->m_ArgCount++;
}

template<typename R>
inline R GetReturn() 
{
	return *(R*)&g_NativeArg->m_ReturnValue[0];
}

template<typename N, typename... A>
N invoke(uint64_t hash, A &&... args)
{
	ResetArgs();
	int dum[] = {0,((void)PushArg(std::forward<A>(args)),0)...};
	((void(*)(NativeArg_s*))(void*)hash)(g_NativeArg);
	SetVec();
	return GetReturn<N>();
}