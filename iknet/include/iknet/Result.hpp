/*
    Copyright (C) 2023, InternationalKoder

    This file is part of IKLibs.

    IKLibs is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    IKLibs is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with IKLibs.  If not, see <https://www.gnu.org/licenses/>.
*/

#ifndef IKNET_RESULT_HPP
#define IKNET_RESULT_HPP

#include <variant>
#include <optional>
#include <concepts>
#include <stdexcept>

namespace iknet
{

using EmptyResult = std::monostate;

/*!
 * \brief A result: either a success or a failure. Success and failure are different types
 */
template<typename S, typename F>
class Result
{
    public:

        /*!
         * \brief Create a success result from an existing value
         * \param success The value contained by the success result
         * \return A new success result containing given value
         */
        static constexpr Result<S, F> success(S&& success) { return Result(std::move(success)); }

        /*!
         * \brief Create a failure result from an existing value
         * \param failure The value contained by the failure result
         * \return A new failure result containing given value
         */
        static constexpr Result<S, F> failure(F&& failure) { return Result(std::move(failure)); }


        /*!
         * \brief Create a success result by constructing a value in place
         * \param args The arguments to construct the success value
         * \return A new success result containing newly constructed value
         */
        template<typename... ARGS>
        static constexpr Result<S, F> makeSuccess(ARGS&&... args) { return Result(std::in_place_type<S>, std::forward<ARGS>(args)...); }

        /*!
         * \brief Create a failure result by constructing a value in place
         * \param args The arguments to construct the failure value
         * \return A new failure result containing newly constructed value
         */
        template<typename... ARGS>
        static constexpr Result<S, F> makeFailure(ARGS&&... args) { return Result(std::in_place_type<F>, std::forward<ARGS>(args)...); }

        /*!
         * \brief Tells whether the result is a success
         * \return True if the result is a success
         */
        constexpr bool isSuccess() const { return std::holds_alternative<S>(m_variant); }

        /*!
         * \brief Tells whether the result is a failure
         * \return True if the result is a failure
         */
        constexpr bool isFailure() const { return std::holds_alternative<F>(m_variant); }


        /*!
         * \brief Gives the success value. Will throw if the result is not success
         * \return A reference to the contained success value
         */
        constexpr const S& getSuccess() const { return std::get<S>(m_variant); }

        /*!
         * \brief Gives the success value. Will throw if the result is not success
         * \return A reference to the contained success value
         */
        constexpr S& getSuccess() { return std::get<S>(m_variant); }


        /*!
         * \brief Gives the failure value. Will throw if the result is not failure
         * \return A reference to the contained failure value
         */
        constexpr const F& getFailure() const { return std::get<F>(m_variant); }

        /*!
         * \brief Gives the failure value. Will throw if the result is not failure
         * \return A reference to the contained failure value
         */
        constexpr F& getFailure() { return std::get<F>(m_variant); }


        /*!
         * \brief Gives the contained value if the result is success, or the value returned by given function otherwise
         * \param invocable A function that will return the value to use if the result is not success
         * \return The contained success value if the result is success, or the value returned by given invocable otherwise
         */
        template<std::invocable I>
        S getSuccessOr(I invocable) const { return isSuccess() ? getSuccess() : invocable(); }

        /*!
         * \brief Gives the contained value if the result is success, or the given default value otherwise
         * \param defaultValue The default value to return if the result is not success
         * \return The contained success value if the result is success, or the given value otherwise
         */
        const S& getSuccessOr(const S& defaultValue) const { return isSuccess() ? getSuccess() : defaultValue; }


        /*!
         * \brief Gives the contained value if the result is failure, or the value returned by given function otherwise
         * \param invocable A function that will return the value to use if the result is not failure
         * \return The contained success value if the result is failure, or the value returned by given invocable otherwise
         */
        template<std::invocable I>
        F getFailureOr(I invocable) const { return isFailure() ? getFailure() : invocable(); }

        /*!
         * \brief Gives the contained value if the result is failure, or the given default value otherwise
         * \param defaultValue The default value to return if the result is not failure
         * \return The contained failure value if the result is failure, or the given value otherwise
         */
        const F& getFailureOr(const F& defaultValue) const { return isFailure() ? getFailure() : defaultValue; }

    private:

        /*!
         * \brief Constructor by moving existing success value
         * \param success The existing success value
         */
        constexpr explicit Result(S&& success) : m_variant(std::move(success)) {}

        /*!
         * \brief Constructor by moving existing failure value
         * \param failure The existing failure value
         */
        constexpr explicit Result(F&& failure) : m_variant(std::move(failure)) {}


        /*!
         * \brief Constructor by constructing a new value in place
         * \param inPlace Indicates that the value should be constructed in place
         * \param args Arguments to construct the new contained value
         */
        template<typename T, typename... ARGS>
        constexpr explicit Result(std::in_place_type_t<T> inPlace, ARGS&&... args) : m_variant(inPlace, std::forward<ARGS>(args)...) {}

        std::variant<S, F> m_variant;
};

/*!
 * \brief A result: either a success or a failure. Success and failure are same type
 */
template<typename SF>
class Result<SF, SF>
{
    public:

        /*!
         * \brief Create a success result from an existing value
         * \param success The value contained by the success result
         * \return A new success result containing given value
         */
        static constexpr Result<SF, SF> success(SF&& value) { return Result(true, std::move(value)); }

        /*!
         * \brief Create a failure result from an existing value
         * \param failure The value contained by the failure result
         * \return A new failure result containing given value
         */
        static constexpr Result<SF, SF> failure(SF&& value) { return Result(false, std::move(value)); }


        /*!
         * \brief Create a success result by constructing a value in place
         * \param args The arguments to construct the success value
         * \return A new success result containing newly constructed value
         */
        template<typename... ARGS>
        static constexpr Result<SF, SF> makeSccess(ARGS&&... args) { return Result(true, std::in_place_type<SF>, std::forward<ARGS>(args)...); }

        /*!
         * \brief Create a failure result by constructing a value in place
         * \param args The arguments to construct the failure value
         * \return A new failure result containing newly constructed value
         */
        template<typename... ARGS>
        static constexpr Result<SF, SF> makeFailure(ARGS&&... args) { return Result(false, std::in_place_type<SF>, std::forward<ARGS>(args)...); }

        /*!
         * \brief Tells whether the result is a success
         * \return True if the result is a success
         */
        constexpr bool isSuccess() const { return m_success; }

        /*!
         * \brief Tells whether the result is a failure
         * \return True if the result is a failure
         */
        constexpr bool isFailure() const { return !m_success; }

        /*!
         * \brief Gives the success value. Will throw if the result is not success
         * \return A reference to the contained success value
         */
        constexpr const SF& getSuccess() const { return m_success ? m_value : throw std::runtime_error("Bad Result access: wanted success but got failure"); }

        /*!
         * \brief Gives the failure value. Will throw if the result is not failure
         * \return A reference to the contained failure value
         */
        constexpr const SF& getFailure() const { return m_success ? throw std::runtime_error("Bad result access: wanted failure but got success") : m_value; }


        /*!
         * \brief Gives the contained value if the result is success, or the value returned by given function otherwise
         * \param invocable A function that will return the value to use if the result is not success
         * \return The contained success value if the result is success, or the value returned by given invocable otherwise
         */
        template<std::invocable I>
        SF getSuccessOr(I invocable) const { return m_success ? m_value : invocable(); }

        /*!
         * \brief Gives the contained value if the result is success, or the given default value otherwise
         * \param defaultValue The default value to return if the result is not success
         * \return The contained success value if the result is success, or the given value otherwise
         */
        const SF& getSuccessOr(const SF& defaultValue) const { return m_success ? m_value : defaultValue; }


        /*!
         * \brief Gives the contained value if the result is failure, or the value returned by given function otherwise
         * \param invocable A function that will return the value to use if the result is not failure
         * \return The contained success value if the result is failure, or the value returned by given invocable otherwise
         */
        template<std::invocable I>
        SF getFailureOr(I invocable) const { return m_success ? invocable() : m_value; }

        /*!
         * \brief Gives the contained value if the result is failure, or the given default value otherwise
         * \param defaultValue The default value to return if the result is not failure
         * \return The contained failure value if the result is failure, or the given value otherwise
         */
        const SF& getFailureOr(const SF& defaultValue) const { return m_success ? defaultValue : m_value; }

    private:

        /*!
         * \brief Constructor by moving existing value
         * \param success Whether the result is a success
         * \param value The existing value
         */
        constexpr Result(bool success, SF&& value) :
            m_success(success),
            m_value(std::move(value))
        {}

        /*!
         * \brief Constructor by constructing a new value in place
         * \param success Whether the result is a success
         * \param inPlace Indicates that the value should be constructed in place
         * \param args Arguments to construct the new contained value
         */
        template<typename T, typename... ARGS>
        constexpr Result(bool success, std::in_place_type_t<T> inPlace, ARGS&&... args) :
            m_success(success),
            m_value(std::forward<ARGS>(args)...)
        {}

        bool m_success;
        SF m_value;
};

}

#endif // IKNET_RESULT_HPP
