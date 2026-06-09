__kernel void AABB(__global const uint* triangles, __global const float* xcoords,__global const float* ycoords, __global const float* zcoords, __global int* usable,__constant float4* raymax, __constant float4* raymin,uint tsize, __global uint* counter) {
    
    uint i;
    i = get_global_id(0);
    if (i >= tsize) return;
    uint idx0 = triangles[i * 3 + 0];
    uint idx1 = triangles[i * 3 + 1];
    uint idx2 = triangles[i * 3 + 2];

    float max_x = max(xcoords[idx0], max(xcoords[idx1], xcoords[idx2]));
    float max_y = max(ycoords[idx0], max(ycoords[idx1], ycoords[idx2]));
    float max_z = max(zcoords[idx0], max(zcoords[idx1], zcoords[idx2]));
    float min_x = min(xcoords[idx0], min(xcoords[idx1], xcoords[idx2]));
    float min_y = min(ycoords[idx0], min(ycoords[idx1], ycoords[idx2]));
    float min_z = min(zcoords[idx0], min(zcoords[idx1], zcoords[idx2]));
    
    if (max_x < raymin->s0 || min_x > raymax->s0) return;
    if (max_y < raymin->s1 || min_y > raymax->s1) return;
    if (max_z < raymin->s2 || min_z > raymax->s2) return;

    uint index = atomic_add(counter, 1);
    usable[index] = i;
}
