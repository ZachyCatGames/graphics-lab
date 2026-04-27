#include <engine/gl/eng_Sphere.h>

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

    static constexpr auto normalizedSphere = CreateNormalizedSpherePoints<4>(normalizedBox);

    std::vector<Vertex> vertices;
    vertices.reserve(normalizedSphere.size());
    for (const auto& vertPos : normalizedSphere) {
        vertices.push_back({
            .position = vertPos * radius,
            .normal = -vertPos,
            .texCoord = {} // TODO
        });
    }
    return vertices;
}

} // namespace

Sphere::Sphere(float radius, const Vector3DF& position, Handle<IShader> shader) :
    Mesh(CreateSphereVBO(radius), position, std::move(shader)),
    m_radius(radius) {}

} // namespace eng::gl
