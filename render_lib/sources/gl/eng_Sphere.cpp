#include <engine/gl/eng_Sphere.h>
#include <cmath>

namespace eng::gl {

namespace {

/* 
 * 36 vertices, for each split this is quadrupled, so our final vertex
 * count is 36 * 4 ^ iterations.
 */
template<size_t I>
using VertexPosList = std::array<Vector3DF, I>;

template<size_t I>
constexpr auto CreateNormalizedSpherePoints(const auto& current) {
    if constexpr (I != 0) {
        auto prev = CreateNormalizedSpherePoints<I-1>(current);

        VertexPosList<prev.size() * 4> expanded;
        size_t expandedIdx = 0;
        static_assert(sizeof(expanded) == sizeof(prev) * 4);
        for (size_t prevIdx = 0; prevIdx < prev.size(); prevIdx += 3) {
            /* For each tri, we'll create four new tri in a triforce pattern. */
            auto v0 = prev[prevIdx + 0].normalize();
            auto v1 = prev[prevIdx + 1].normalize();
            auto v2 = prev[prevIdx + 2].normalize();
            auto ve01 = ((v0 + v1) / 2.0f).normalize();
            auto ve12 = ((v1 + v2) / 2.0f).normalize();
            auto ve20 = ((v2 + v0) / 2.0f).normalize();

            std::ranges::copy(std::array{ v0, ve01, ve20 },   &expanded[expandedIdx + 0]);
            std::ranges::copy(std::array{ ve01, v1, ve12 },   &expanded[expandedIdx + 3]);
            std::ranges::copy(std::array{ ve20, v2, ve12 },   &expanded[expandedIdx + 6]);
            std::ranges::copy(std::array{ ve01, ve12, ve20 }, &expanded[expandedIdx + 9]);
            expandedIdx += 12;
        }

        return expanded;
    } else {
        return current;
    }
}

template<size_t N>
constexpr std::array<Vertex, N> CreateSphereNormalsAndUVs(const VertexPosList<N>& positions) {
    std::array<Vertex, N> verts = {};

    for (size_t i = 0; i < N; i++) {
        const auto& pos = positions[i];

        /*
         * atan2 can't differentiate between vertices on X=0.0 that belong
         * to triangles going toward positive X and negative X and will
         * return 1.0 in either case.
         * This causes problems with triangle going toward negative X
         * because their U coordinate will suddently jump from a value close
         * to close on their non-zero X vertices to 1.0 on their vertices located
         * at X=0.0.
         *
         * To work around this, when we encounter a vertex at X=0.0 we check
         * if the previous either of the previous two vertices were at a 
         * negative X coordinate. If they were, we shift the coordinate slightly
         * toward negative X
         */
        float texX = pos.x();
        if (i > 1 && (positions[i-1].x() < 0.0 || positions[i-2].x() < 0.0) && texX == 0.0) {
            texX = -0.001f;
        }

        const float texU = (std::atan2(texX, pos.z() + 0.001) + std::numbers::pi) / (2 * std::numbers::pi);
        const float texV = -std::acos(pos.y()) / std::numbers::pi;

        verts[i] = {
            .position = pos,
            .normal   = pos,
            .texCoord = { texU, texV }
        };
    }

    return verts;
}

constexpr std::vector<Vertex> CreateSphereVBO(float radius) {
    /* There's probably a more efficient way to do this but lol. */
    static constexpr auto normalizedBox = std::to_array<Vector3DF>({
        // Front face (looking down -Z)
        { -1.0f, -1.0f, -1.0f }, // v1
        {  1.0f, -1.0f, -1.0f },
        {  1.0f,  1.0f, -1.0f },
        { -1.0f, -1.0f, -1.0f }, // v2
        {  1.0f,  1.0f, -1.0f },
        { -1.0f,  1.0f, -1.0f },

        // Right face
        {  1.0f, -1.0f, -1.0f }, // v1
        {  1.0f, -1.0f,  1.0f },
        {  1.0f,  1.0f,  1.0f },
        {  1.0f, -1.0f, -1.0f }, // v2
        {  1.0f,  1.0f,  1.0f },
        {  1.0f,  1.0f, -1.0f },

        // Back face
        {  1.0f, -1.0f,  1.0f }, // v1
        { -1.0f, -1.0f,  1.0f },
        { -1.0f,  1.0f,  1.0f },
        {  1.0f, -1.0f,  1.0f }, // v2
        { -1.0f,  1.0f,  1.0f },
        {  1.0f,  1.0f,  1.0f },

        // Left face
        { -1.0f, -1.0f,  1.0f }, // v1
        { -1.0f, -1.0f, -1.0f },
        { -1.0f,  1.0f, -1.0f },
        { -1.0f, -1.0f,  1.0f }, // v2
        { -1.0f,  1.0f, -1.0f },
        { -1.0f,  1.0f,  1.0f },

        // Top face
        { -1.0f,  1.0f, -1.0f }, // v1
        {  1.0f,  1.0f, -1.0f },
        {  1.0f,  1.0f,  1.0f },
        { -1.0f,  1.0f, -1.0f }, // v2
        {  1.0f,  1.0f,  1.0f },
        { -1.0f,  1.0f,  1.0f },

        // Bottom face
        { -1.0f,  -1.0f,  1.0f }, // v1
        {  1.0f,  -1.0f,  1.0f },
        {  1.0f,  -1.0f, -1.0f },
        { -1.0f,  -1.0f,  1.0f }, // v2
        {  1.0f,  -1.0f, -1.0f },
        { -1.0f,  -1.0f, -1.0f },
    });

    //static constexpr auto normalizedSphere = CreateNormalizedSpherePoints<4>(normalizedBox);
    static constexpr auto normalizedSphere = CreateSphereNormalsAndUVs(CreateNormalizedSpherePoints<4>(normalizedBox));
    
    #if 0
    for (const auto& v : normalizedSphere) {
        std::printf("{ %f %f %f } ", v.position.x(), v.position.y(), v.position.z());

        std::printf("{ %f %f }\n", v.texCoord.x(), v.texCoord.y());
    }
    #endif

    return std::vector<Vertex>(std::from_range_t(), normalizedSphere);
}

} // namespace

Sphere::Sphere(float radius, const Vector3DF& position, Handle<IShader> shader) :
    Mesh(CreateSphereVBO(radius), position, std::move(shader)),
    m_radius(radius) {}

} // namespace eng::gl
