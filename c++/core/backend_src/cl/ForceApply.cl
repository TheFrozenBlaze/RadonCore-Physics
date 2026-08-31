__kernel void fa_kernel(__global float* coords,  __constant float3* force)
{
uint i;
i = get_global_id(0);
coords[i*3] += force->x;
coords[i*3+1] += force->y;
coords[i*3+2] += force->z;
};