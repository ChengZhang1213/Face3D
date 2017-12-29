/*
 * eos - A 3D Morphable Model fitting library written in modern C++11/14.
 *
 * File: include/eos/render/FragmentShader.hpp
 *
 * Copyright 2017 Patrik Huber
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#pragma once

#ifndef FRAGMENTSHADER_HPP_
#define FRAGMENTSHADER_HPP_

#include "eos/render/detail/Vertex.hpp"
#include "eos/render/utils.hpp"

#include "glm/vec3.hpp"
#include "glm/vec4.hpp"

#include "boost/optional.hpp"

// Fragment shaders are a more accurate name for the same functionality as Pixel shaders. They aren't pixels
// yet, since the output still has to past several tests (depth, alpha, stencil) as well as the fact that one
// may be using antialiasing, which renders one-fragment-to-one-pixel non-true.
// The "pixel" in "pixel shader" is a misnomer because the pixel shader doesn't operate on pixels directly.
// The pixel shader operates on "fragments" which may or may not end up as actual pixels, depending on several
// factors outside of the pixel shader.
// The shaders *can not* depend on any state - they have to be able to run independently and in parallel!
// But can they really? What about the z-test? It happens earlier at the moment - which is good?

namespace eos {
namespace render {

/**
 * @brief A simple fragment shader that does vertex-colouring.
 *
 * Uses the vertex colour data to shade the given fragment / pixel location.
 */
class VertexColoringFragmentShader
{
public:
    /**
     * @brief Todo.
     *
     * X
     * lambda is not perspectively corrected. Note: In our case, it is, as we do it in the raster loop at
     * the moment?
     * Note/TODO: We should document in what range we expect ::color to be! Eg specify that Mesh should be
     * [0,255]? But don't we then lose precision sometimes (e.g. superres)? I think we can leave Mesh /
     * everything in [0,1] and convert at the very end.
     *
     * @param[in] x X.
     * @ return RGBA... in [0, 1]?
     */
    template <typename T, glm::precision P = glm::defaultp>
    glm::tvec4<T, P> shade_triangle_pixel(int x, int y, const detail::Vertex<T, P>& point_a,
                                          const detail::Vertex<T, P>& point_b,
                                          const detail::Vertex<T, P>& point_c, const glm::tvec3<T, P>& lambda,
                                          const boost::optional<Texture>& texture, float dudx, float dudy,
                                          float dvdx, float dvdy)
    {
        // attributes interpolation
        glm::tvec3<T, P> color_persp =
            lambda[0] * point_a.color + lambda[1] * point_b.color + lambda[2] * point_c.color;
        return glm::tvec4<T, P>(color_persp, T(1));
    };
};

/**
 * @brief A fragment shader that textures...
 *
 * X.
 */
class TexturingFragmentShader
{
public:
    /**
     * @brief Todo.
     *
     * See comments above about lambda (persp. corrected?) and the colour range.
     *
     * @param[in] x X.
     * @ return RGBA... in [0, 1]?
     */
    template <typename T, glm::precision P = glm::defaultp>
    glm::tvec4<T, P> shade_triangle_pixel(int x, int y, const detail::Vertex<T, P>& point_a,
                                          const detail::Vertex<T, P>& point_b,
                                          const detail::Vertex<T, P>& point_c, const glm::tvec3<T, P>& lambda,
                                          const boost::optional<eos::render::Texture>& texture, float dudx,
                                          float dudy, float dvdx, float dvdy)
    {
        glm::tvec2<T, P> texcoords_persp =
            lambda[0] * point_a.texcoords + lambda[1] * point_b.texcoords + lambda[2] * point_c.texcoords;

        // The Texture is in BGR, thus tex2D returns BGR
        // Todo: Think about changing that.
        glm::tvec3<T, P> texture_color =
            detail::tex2d(texcoords_persp, texture.get(), dudx, dudy, dvdx, dvdy); // uses the current texture
        glm::tvec3<T, P> pixel_color = glm::tvec3<T, P>(texture_color[2], texture_color[1], texture_color[0]);
        // other: color.mul(tex2D(texture, texCoord));
        return glm::tvec4<T, P>(pixel_color, T(1));
    };
};

/**
 * @brief X.
 *
 * X.
 * Inverts the perspective texture mapping. Can be derived using some tedious algebra.
 * Todo: Probably move to a texturing file, internal/detail one, where we will also put the tex2d, mipmapping
 * etc stuff?
 *
 * @param[in] X X.
 * @return X.
 */
template <typename T, glm::precision P = glm::defaultp>
glm::tvec3<T, P> compute_inverse_perspectively_correct_lambda(const glm::tvec3<T, P>& lambda_world,
                                                              const T& one_over_w0, const T& one_over_w1,
                                                              const T& one_over_w2)
{
    float w0 = 1 / one_over_w0;
    float w1 = 1 / one_over_w1;
    float w2 = 1 / one_over_w2;

    float d = w0 - (w0 - w1) * lambda_world.y - (w0 - w2) * lambda_world.z;
    if (d == 0)
        return lambda_world;

    glm::tvec3<T, P> lambda;

    lambda.z = lambda_world.z * w2 / d;
    lambda.y = lambda_world.y * w1 / d;

    lambda.x = 1 - lambda.y - lambda.z;
    return lambda;
};

class ExtractionFragmentShader
{
public:
    /**
     * @brief X.
     *
     * X.
     * Inverts the perspective texture mapping. Can be derived using some tedious algebra.
     * NOTE: This one actually takes/needs the perspectively corrected lambda I think!
     *
     * Todo: Probably move to a texturing file, internal/detail one, where we will also put the tex2d,
     * mipmapping etc stuff?
     *
     * @param[in] X X.
     * @return X.
     */
    template <typename T, glm::precision P = glm::defaultp>
    glm::tvec4<T, P> shade_triangle_pixel(int x, int y, const detail::Vertex<T, P>& point_a,
                                          const detail::Vertex<T, P>& point_b,
                                          const detail::Vertex<T, P>& point_c, const glm::tvec3<T, P>& lambda,
                                          const boost::optional<Texture>& texture, float dudx, float dudy,
                                          float dvdx, float dvdy)
    {
        auto corrected_lambda = compute_inverse_perspectively_correct_lambda(
            lambda, point_a.position.w, point_b.position.w, point_c.position.w);
        glm::tvec2<T, P> texcoords_persp = corrected_lambda[0] * point_a.texcoords +
                                           corrected_lambda[1] * point_b.texcoords +
                                           corrected_lambda[2] * point_c.texcoords;

        // Texturing, no mipmapping:
        cv::Vec2f image_tex_coords = detail::texcoord_wrap(cv::Vec2f(texcoords_persp.s, texcoords_persp.t));
        image_tex_coords[0] *= texture->mipmaps[0].cols;
        image_tex_coords[1] *= texture->mipmaps[0].rows;
        cv::Vec3f texture_color = detail::tex2d_linear(image_tex_coords, 0, texture.get()) / 255.0;
        glm::tvec3<T, P> pixel_color = glm::tvec3<T, P>(texture_color[2], texture_color[1], texture_color[0]);
        return glm::tvec4<T, P>(pixel_color, T(1));
    };
};

} /* namespace render */
} /* namespace eos */

#endif /* FRAGMENTSHADER_HPP_ */
