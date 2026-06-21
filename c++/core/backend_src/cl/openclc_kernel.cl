__kernel void AABB(__global const uint* triangles, __global const float* coords, __global int* usable,__constant float4* raymax, __constant float4* raymin,uint tsize, __global uint* counter) {
    
    uint i;
    i = get_global_id(0);
    if (i >= tsize) return;
    uint idx0 = triangles[i * 3 + 0];
    uint idx1 = triangles[i * 3 + 1];
    uint idx2 = triangles[i * 3 + 2];

    float max_x = max(coords[idx0*3], max(coords[idx1*3 ], coords[idx2*3]));
    float max_y = max(coords[idx0*3 +1], max(coords[idx1*3 +1], coords[idx2*3 + 1]));
    float max_z = max(coords[idx0*3 +2], max(coords[idx1*3+2], coords[idx2*3 +2]));
    float min_x = min(coords[idx0*3], min(coords[idx1*3 ], coords[idx2*3]));
    float min_y = min(coords[idx0*3 +1], min(coords[idx1*3 +1], coords[idx2*3 + 1]));
    float min_z = min(coords[idx0*3 +2], min(coords[idx1*3+2], coords[idx2*3 +2]));
    
    if (max_x < raymin->s0 || min_x > raymax->s0) return;
    if (max_y < raymin->s1 || min_y > raymax->s1) return;
    if (max_z < raymin->s2 || min_z > raymax->s2) return;

    uint index = atomic_add(counter, 1);
    usable[index] = i;
}
