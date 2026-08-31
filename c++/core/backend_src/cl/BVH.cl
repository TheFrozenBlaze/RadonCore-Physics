typedef struct {
    uint triangle_id;
    uint ray_id;
    float3 hit_pos;
} Hit;

__kernel void BVH(
    __global const uint* triangles,
    __global const float* coords,
    __global Hit* usable,
    __global const float4* ray,     // Using float4 to match 16-byte alignment
    __global const float4* raystp,  // Using float4 to match 16-byte alignment
    __global uint* counter) 
{
    size_t i = get_global_id(0); 
    size_t r = get_global_id(1); 

    uint v0_idx = triangles[i * 3 + 0];
    uint v1_idx = triangles[i * 3 + 1];
    uint v2_idx = triangles[i * 3 + 2];

    float3 v0 = (float3)(coords[v0_idx * 3 + 0], coords[v0_idx * 3 + 1], coords[v0_idx * 3 + 2]);
    float3 v1 = (float3)(coords[v1_idx * 3 + 0], coords[v1_idx * 3 + 1], coords[v1_idx * 3 + 2]);
    float3 v2 = (float3)(coords[v2_idx * 3 + 0], coords[v2_idx * 3 + 1], coords[v2_idx * 3 + 2]);

    float3 O = ray[r].xyz;
    float3 End = raystp[r].xyz;
    float3 D = End - O;
    float max_dist = length(D); // Distance from start to end
    D = normalize(D);           // Ray direction

    float EPSILON = 1e-6f;
    float3 edge1 = v1 - v0;
    float3 edge2 = v2 - v0;
    float3 h = cross(D, edge2);
    float a = dot(edge1, h);

    if (fabs(a) < EPSILON) {
        return; 
    }

    float f = 1.0f / a;
    float3 s = O - v0;
    float u = f * dot(s, h);

    if (u < 0.0f || u > 1.0f) {
        return;
    }

    float3 q = cross(s, edge1);
    float v = f * dot(D, q);

    if (v < 0.0f || u + v > 1.0f) {
        return;
    }

    float t = f * dot(edge2, q);


    if (t > EPSILON && t <= max_dist) {
        
        uint hit_index = atomic_inc(counter);
        
        // Write the hit data
        usable[hit_index].triangle_id = (uint)i;
        usable[hit_index].ray_id = (uint)r;
        usable[hit_index].hit_pos = O + (D * t);
    }
}
