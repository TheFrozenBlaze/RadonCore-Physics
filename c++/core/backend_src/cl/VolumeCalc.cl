__kernel void volume_calc(__global const float* coords, __global const uint* triangles, __global float4* out)
{
    uint i = get_global_id(0);
    uint3 idx = vload3(i, triangles);

    float3 A = vload3(idx.x, coords);
    float3 B = vload3(idx.y, coords);
    float3 C = vload3(idx.z, coords);

    float volume = dot(A, cross(B, C)) / 6.0f;
    float3 centroid = (A + B + C) * 0.25f;

    out[i] = (float4)(centroid * volume, volume);
};