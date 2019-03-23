/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// TGUI - Texus' Graphical User Interface
// Copyright (C) 2012-2018 Bruno Van de Velde (vdv_b@tgui.eu)
//
// This software is provided 'as-is', without any express or implied warranty.
// In no event will the authors be held liable for any damages arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it freely,
// subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented;
//    you must not claim that you wrote the original software.
//    If you use this software in a product, an acknowledgment
//    in the product documentation would be appreciated but is not required.
//
// 2. Altered source versions must be plainly marked as such,
//    and must not be misrepresented as being the original software.
//
// 3. This notice may not be removed or altered from any source distribution.
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


#include <TGUI/Clipping.hpp>
#include <cmath>

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace tgui
{
    sf::View Clipping::m_originalView;

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    Clipping::Clipping(sf::RenderTarget& target, const sf::RenderStates& states, Vector2f topLeft, Vector2f size) :
        m_target {target},
        m_oldView{target.getView()}
    {
        Vector2f bottomRight = Vector2f(states.transform.transformPoint(topLeft + size));
        topLeft = Vector2f(states.transform.transformPoint(topLeft));

        Vector2f viewTopLeft = topLeft;
        size = bottomRight - topLeft;

        topLeft.x -= m_originalView.getCenter().x - (m_originalView.getSize().x / 2.f);
        topLeft.y -= m_originalView.getCenter().y - (m_originalView.getSize().y / 2.f);
        bottomRight.x -= m_originalView.getCenter().x - (m_originalView.getSize().x / 2.f);
        bottomRight.y -= m_originalView.getCenter().y - (m_originalView.getSize().y / 2.f);

        topLeft.x *= m_originalView.getViewport().width / m_originalView.getSize().x;
        topLeft.y *= m_originalView.getViewport().height / m_originalView.getSize().y;
        size.x *= m_originalView.getViewport().width / m_originalView.getSize().x;
        size.y *= m_originalView.getViewport().height / m_originalView.getSize().y;

        topLeft.x += m_originalView.getViewport().left;
        topLeft.y += m_originalView.getViewport().top;

        if (topLeft.x < m_oldView.getViewport().left)
        {
            size.x -= m_oldView.getViewport().left - topLeft.x;
            viewTopLeft.x += (m_oldView.getViewport().left - topLeft.x) * (m_originalView.getSize().x / m_originalView.getViewport().width);
            topLeft.x = m_oldView.getViewport().left;
        }
        if (topLeft.y < m_oldView.getViewport().top)
        {
            size.y -= m_oldView.getViewport().top - topLeft.y;
            viewTopLeft.y += (m_oldView.getViewport().top - topLeft.y) * (m_originalView.getSize().y / m_originalView.getViewport().height);
            topLeft.y = m_oldView.getViewport().top;
        }

        if (size.x > m_oldView.getViewport().left + m_oldView.getViewport().width - topLeft.x)
            size.x = m_oldView.getViewport().left + m_oldView.getViewport().width - topLeft.x;
        if (size.y > m_oldView.getViewport().top + m_oldView.getViewport().height - topLeft.y)
            size.y = m_oldView.getViewport().top + m_oldView.getViewport().height - topLeft.y;

        if ((size.x >= 0) && (size.y >= 0))
        {
            sf::View view{{std::round(viewTopLeft.x),
                           std::round(viewTopLeft.y),
                           std::round(size.x * m_originalView.getSize().x / m_originalView.getViewport().width),
                           std::round(size.y * m_originalView.getSize().y / m_originalView.getViewport().height)}};

            view.setViewport({topLeft.x, topLeft.y, size.x, size.y});
            target.setView(view);
        }
        else // The clipping area lies outside the viewport
        {
            sf::View emptyView{{0, 0, 0, 0}};
            emptyView.setViewport({0, 0, 0, 0});
            target.setView(emptyView);
        }
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    Clipping::~Clipping()
    {
        m_target.setView(m_oldView);
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void Clipping::setGuiView(const sf::View& view)
    {
        m_originalView = view;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// TGUI - Texus' Graphical User Interface
// Copyright (C) 2012-2018 Bruno Van de Velde (vdv_b@tgui.eu)
//
// This software is provided 'as-is', without any express or implied warranty.
// In no event will the authors be held liable for any damages arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it freely,
// subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented;
//    you must not claim that you wrote the original software.
//    If you use this software in a product, an acknowledgment
//    in the product documentation would be appreciated but is not required.
//
// 2. Altered source versions must be plainly marked as such,
//    and must not be misrepresented as being the original software.
//
// 3. This notice may not be removed or altered from any source distribution.
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


#include <TGUI/CustomWidgetForBindings.hpp>

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace tgui
{
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    CustomWidgetForBindings::Ptr CustomWidgetForBindings::create()
    {
        return std::make_shared<CustomWidgetForBindings>();
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void CustomWidgetForBindings::setPosition(const Layout2d& position)
    {
        Widget::setPosition(position);
        implPositionChanged(position.getValue());
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void CustomWidgetForBindings::setSize(const Layout2d& size)
    {
        Widget::setSize(size);
        implSizeChanged(size.getValue());
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    Vector2f CustomWidgetForBindings::getFullSize() const
    {
        return implGetFullSize();
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    Vector2f CustomWidgetForBindings::getAbsolutePosition() const
    {
        return implGetAbsolutePosition();
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    Vector2f CustomWidgetForBindings::getWidgetOffset() const
    {
        return implGetWidgetOffset();
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void CustomWidgetForBindings::setVisible(bool visible)
    {
        Widget::setVisible(visible);
        implVisibleChanged(visible);
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void CustomWidgetForBindings::setEnabled(bool enabled)
    {
        Widget::setEnabled(enabled);
        implEnableChanged(enabled);
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void CustomWidgetForBindings::setFocused(bool focused)
    {
        Widget::setFocused(focused);
        implFocusChanged(focused);
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    bool CustomWidgetForBindings::canGainFocus() const
    {
        return implCanGainFocus();
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void CustomWidgetForBindings::update(sf::Time elapsedTime)
    {
        Widget::update(elapsedTime);
        implUpdateFunction(elapsedTime);
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    bool CustomWidgetForBindings::mouseOnWidget(Vector2f pos) const
    {
        return implMouseOnWidget(pos);
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void CustomWidgetForBindings::leftMousePressed(Vector2f pos)
    {
        Widget::leftMousePressed(pos);
        implLeftMousePressed(pos);
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void CustomWidgetForBindings::leftMouseReleased(Vector2f pos)
    {
        Widget::leftMouseReleased(pos);
        implLeftMouseReleased(pos);
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void CustomWidgetForBindings::rightMousePressed(Vector2f pos)
    {
        Widget::rightMousePressed(pos);
        implRightMousePressed(pos);
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void CustomWidgetForBindings::rightMouseReleased(Vector2f pos)
    {
        Widget::rightMouseReleased(pos);
        implRightMouseReleased(pos);
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void CustomWidgetForBindings::mouseMoved(Vector2f pos)
    {
        Widget::mouseMoved(pos);
        implMouseMoved(pos);
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void CustomWidgetForBindings::keyPressed(const sf::Event::KeyEvent& event)
    {
        Widget::keyPressed(event);
        implKeyPressed(event);
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void CustomWidgetForBindings::textEntered(std::uint32_t key)
    {
        Widget::textEntered(key);
        implTextEntered(key);
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    bool CustomWidgetForBindings::mouseWheelScrolled(float delta, Vector2f pos)
    {
        return implMouseWheelScrolled(delta, pos);
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void CustomWidgetForBindings::mouseNoLongerOnWidget()
    {
        Widget::mouseNoLongerOnWidget();
        implMouseNoLongerOnWidget();
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void CustomWidgetForBindings::mouseNoLongerDown()
    {
        Widget::mouseNoLongerDown();
        implMouseNoLongerDown();
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void CustomWidgetForBindings::rendererChanged(const std::string& property)
    {
        if (!implRendererChanged(property))
            Widget::rendererChanged(property);
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void CustomWidgetForBindings::mouseEnteredWidget()
    {
        Widget::mouseEnteredWidget();
        implMouseEnteredWidget();
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void CustomWidgetForBindings::mouseLeftWidget()
    {
        Widget::mouseLeftWidget();
        implMouseLeftWidget();
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void CustomWidgetForBindings::draw(sf::RenderTarget& target, sf::RenderStates states) const
    {
        implDrawFunction(target, states);
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// TGUI - Texus' Graphical User Interface
// Copyright (C) 2012-2018 Bruno Van de Velde (vdv_b@tgui.eu)
//
// This software is provided 'as-is', without any express or implied warranty.
// In no event will the authors be held liable for any damages arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it freely,
// subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented;
//    you must not claim that you wrote the original software.
//    If you use this software in a product, an acknowledgment
//    in the product documentation would be appreciated but is not required.
//
// 2. Altered source versions must be plainly marked as such,
//    and must not be misrepresented as being the original software.
//
// 3. This notice may not be removed or altered from any source distribution.
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <TGUI/Gui.hpp>
#include <TGUI/Clipboard.hpp>
#include <TGUI/ToolTip.hpp>
#include <TGUI/Clipping.hpp>

#include <SFML/Graphics/RenderTexture.hpp>

#include <cassert>

#ifdef SFML_SYSTEM_WINDOWS
    #ifndef NOMINMAX // MinGW already defines this which causes a warning without this check
        #define NOMINMAX
    #endif
    #define NOMB
    #define VC_EXTRALEAN
    #define WIN32_LEAN_AND_MEAN
    #include <Windows.h>
#endif

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace tgui
{
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    Gui::Gui() :
    #if SFML_VERSION_MAJOR > 2 || (SFML_VERSION_MAJOR == 2 && SFML_VERSION_MINOR >= 5)
        m_target(nullptr)
    #else
        m_target        (nullptr),
        m_accessToWindow(false)
    #endif
    {
        init();
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#if SFML_VERSION_MAJOR == 2 && SFML_VERSION_MINOR < 5
    Gui::Gui(sf::RenderWindow& window) :
        m_target        (&window),
        m_accessToWindow(true)
    {
        Clipboard::setWindowHandle(window.getSystemHandle());

        setView(window.getDefaultView());

        init();
    }
#endif

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    Gui::Gui(sf::RenderTarget& target) :
    #if SFML_VERSION_MAJOR > 2 || (SFML_VERSION_MAJOR == 2 && SFML_VERSION_MINOR >= 5)
        m_target(&target)
    #else
        m_target        (&target),
        m_accessToWindow(false)
    #endif
    {
        setView(target.getDefaultView());

        init();
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#if SFML_VERSION_MAJOR == 2 && SFML_VERSION_MINOR < 5
    void Gui::setTarget(sf::RenderWindow& window)
    {
        m_target = &window;

        m_accessToWindow = true;
        Clipboard::setWindowHandle(window.getSystemHandle());

        setView(window.getDefaultView());
    }
#endif

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void Gui::setTarget(sf::RenderTarget& target)
    {
    #if SFML_VERSION_MAJOR == 2 && SFML_VERSION_MINOR < 5
        m_accessToWindow = false;
    #endif

        m_target = &target;

        setView(target.getDefaultView());
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    sf::RenderTarget* Gui::getTarget() const
    {
        return m_target;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void Gui::setView(const sf::View& view)
    {
        if ((m_view.getCenter() != view.getCenter()) || (m_view.getSize() != view.getSize()))
        {
            m_view = view;

            m_container->m_size = view.getSize();
            m_container->onSizeChange.emit(m_container.get(), m_container->getSize());

            for (auto& layout : m_container->m_boundSizeLayouts)
                layout->recalculateValue();
        }
        else // Set it anyway in case something changed that we didn't care to check
            m_view = view;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    const sf::View& Gui::getView() const
    {
        return m_view;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    bool Gui::handleEvent(sf::Event event)
    {
        assert(m_target != nullptr);

        // Check if the event has something to do with the mouse
        if ((event.type == sf::Event::MouseMoved) || (event.type == sf::Event::TouchMoved)
         || (event.type == sf::Event::MouseButtonPressed) || (event.type == sf::Event::TouchBegan)
         || (event.type == sf::Event::MouseButtonReleased) || (event.type == sf::Event::TouchEnded)
         || (event.type == sf::Event::MouseWheelScrolled))
        {
            Vector2f mouseCoords;

            switch (event.type)
            {
                case sf::Event::MouseMoved:
                {
                    mouseCoords = m_target->mapPixelToCoords({event.mouseMove.x, event.mouseMove.y}, m_view);
                    event.mouseMove.x = static_cast<int>(mouseCoords.x + 0.5f);
                    event.mouseMove.y = static_cast<int>(mouseCoords.y + 0.5f);
                    break;
                }

                case sf::Event::MouseButtonPressed:
                case sf::Event::MouseButtonReleased:
                {
                    mouseCoords = m_target->mapPixelToCoords({event.mouseButton.x, event.mouseButton.y}, m_view);
                    event.mouseButton.x = static_cast<int>(mouseCoords.x + 0.5f);
                    event.mouseButton.y = static_cast<int>(mouseCoords.y + 0.5f);
                    break;
                }

                case sf::Event::MouseWheelScrolled:
                {
                    mouseCoords = m_target->mapPixelToCoords({event.mouseWheelScroll.x, event.mouseWheelScroll.y}, m_view);
                    event.mouseWheelScroll.x = static_cast<int>(mouseCoords.x + 0.5f);
                    event.mouseWheelScroll.y = static_cast<int>(mouseCoords.y + 0.5f);
                    break;
                }

                case sf::Event::TouchMoved:
                case sf::Event::TouchBegan:
                case sf::Event::TouchEnded:
                {
                    mouseCoords = m_target->mapPixelToCoords({event.touch.x, event.touch.y}, m_view);
                    event.touch.x = static_cast<int>(mouseCoords.x + 0.5f);
                    event.touch.y = static_cast<int>(mouseCoords.y + 0.5f);
                    break;
                }

                default:
                    break;
            }

            // If a tooltip is visible then hide it now
            if (m_visibleToolTip != nullptr)
            {
                // Correct the position of the tool tip so that it is relative again
                m_visibleToolTip->setPosition(m_visibleToolTip->getPosition() - ToolTip::getDistanceToMouse() - m_lastMousePos);

                remove(m_visibleToolTip);
                m_visibleToolTip = nullptr;
            }

            // Reset the data for the tooltip since the mouse has moved
            m_tooltipTime = {};
            m_tooltipPossible = true;
            m_lastMousePos = mouseCoords;
        }

        // Handle tab key presses
        else if (event.type == sf::Event::KeyPressed)
        {
            if (isTabKeyUsageEnabled() && (event.key.code == sf::Keyboard::Tab))
            {
                if (event.key.shift)
                    focusPreviousWidget();
                else
                    focusNextWidget();

                return true;
            }
        }

        // Keep track of whether the window is focused or not
        else if (event.type == sf::Event::LostFocus)
        {
            m_windowFocused = false;
        }
        else if (event.type == sf::Event::GainedFocus)
        {
            m_windowFocused = true;
        #if SFML_VERSION_MAJOR == 2 && SFML_VERSION_MINOR < 5
            if (m_accessToWindow)
                Clipboard::setWindowHandle(static_cast<sf::RenderWindow*>(m_target)->getSystemHandle());
        #endif
        }

        return m_container->handleEvent(event);
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void Gui::enableTabKeyUsage()
    {
        m_TabKeyUsageEnabled = true;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void Gui::disableTabKeyUsage()
    {
        m_TabKeyUsageEnabled = false;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    bool Gui::isTabKeyUsageEnabled() const
    {
        return m_TabKeyUsageEnabled;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void Gui::draw()
    {
        assert(m_target != nullptr);

        // Update the time
        if (m_windowFocused)
            updateTime(m_clock.restart());
        else
            m_clock.restart();

        // Change the view
        const sf::View oldView = m_target->getView();
        m_target->setView(m_view);
        Clipping::setGuiView(m_view);

        // Draw the widgets
        m_container->drawWidgetContainer(m_target, sf::RenderStates::Default);

        // Restore the old view
        m_target->setView(oldView);
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    GuiContainer::Ptr Gui::getContainer() const
    {
        return m_container;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void Gui::setFont(const Font& font)
    {
        m_container->setInheritedFont(font);
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    std::shared_ptr<sf::Font> Gui::getFont() const
    {
        return m_container->getInheritedFont();
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    const std::vector<Widget::Ptr>& Gui::getWidgets() const
    {
        return m_container->getWidgets();
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    const std::vector<sf::String>& Gui::getWidgetNames() const
    {
        return m_container->getWidgetNames();
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void Gui::add(const Widget::Ptr& widgetPtr, const sf::String& widgetName)
    {
        m_container->add(widgetPtr, widgetName);
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    Widget::Ptr Gui::get(const sf::String& widgetName) const
    {
        return m_container->get(widgetName);
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    bool Gui::remove(const Widget::Ptr& widget)
    {
        return m_container->remove(widget);
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void Gui::removeAllWidgets()
    {
        m_container->removeAllWidgets();
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    bool Gui::setWidgetName(const Widget::Ptr& widget, const std::string& name)
    {
        return m_container->setWidgetName(widget, name);
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    std::string Gui::getWidgetName(const Widget::Ptr& widget) const
    {
        return m_container->getWidgetName(widget);
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void Gui::focusNextWidget()
    {
        m_container->focusNextWidget();
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void Gui::focusPreviousWidget()
    {
        m_container->focusPreviousWidget();
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void Gui::unfocusAllWidgets()
    {
        m_container->setFocused(false);
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void Gui::uncheckRadioButtons()
    {
        m_container->uncheckRadioButtons();
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void Gui::setOpacity(float opacity)
    {
        m_container->setInheritedOpacity(opacity);
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    float Gui::getOpacity() const
    {
        return m_container->getInheritedOpacity();
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void Gui::loadWidgetsFromFile(const std::string& filename)
    {
        m_container->loadWidgetsFromFile(filename);
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void Gui::saveWidgetsToFile(const std::string& filename)
    {
        m_container->saveWidgetsToFile(filename);
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void Gui::loadWidgetsFromStream(std::stringstream& stream)
    {
        m_container->loadWidgetsFromStream(stream);
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void Gui::loadWidgetsFromStream(std::stringstream&& stream)
    {
        loadWidgetsFromStream(stream);
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void Gui::saveWidgetsToStream(std::stringstream& stream) const
    {
        m_container->saveWidgetsToStream(stream);
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void Gui::updateTime(const sf::Time& elapsedTime)
    {
        m_container->m_animationTimeElapsed = elapsedTime;
        m_container->update(elapsedTime);

        if (m_tooltipPossible)
        {
            m_tooltipTime += elapsedTime;
            if (m_tooltipTime >= ToolTip::getTimeToDisplay())
            {
                Widget::Ptr tooltip = m_container->askToolTip(m_lastMousePos);
                if (tooltip)
                {
                    m_visibleToolTip = tooltip;
                    add(tooltip, "#TGUI_INTERNAL$ToolTip#");

                    // Change the relative tool tip position in an absolute one
                    tooltip->setPosition(m_lastMousePos + ToolTip::getDistanceToMouse() + tooltip->getPosition());
                }

                m_tooltipPossible = false;
            }
        }
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void Gui::init()
    {
    #ifdef SFML_SYSTEM_WINDOWS
        unsigned int doubleClickTime = GetDoubleClickTime();
        if (doubleClickTime > 0)
            setDoubleClickTime(doubleClickTime);
    #endif
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// TGUI - Texus' Graphical User Interface
// Copyright (C) 2012-2018 Bruno Van de Velde (vdv_b@tgui.eu)
//
// This software is provided 'as-is', without any express or implied warranty.
// In no event will the authors be held liable for any damages arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it freely,
// subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented;
//    you must not claim that you wrote the original software.
//    If you use this software in a product, an acknowledgment
//    in the product documentation would be appreciated but is not required.
//
// 2. Altered source versions must be plainly marked as such,
//    and must not be misrepresented as being the original software.
//
// 3. This notice may not be removed or altered from any source distribution.
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


#include <TGUI/Sprite.hpp>
#include <TGUI/Color.hpp>
#include <TGUI/Clipping.hpp>

#include <cassert>

#ifdef TGUI_USE_CPP17
    #include <optional>
#endif

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace tgui
{
    void Sprite::setTexture(const Texture& texture)
    {
        m_texture = texture;

        if (isSet())
        {
            if (getSize() == Vector2f{})
                setSize(texture.getImageSize());
            else
                updateVertices();
        }
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    const Texture& Sprite::getTexture() const
    {
        return m_texture;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    Texture& Sprite::getTexture()
    {
        return m_texture;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    bool Sprite::isSet() const
    {
        return m_texture.getData() != nullptr;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void Sprite::setSize(const Vector2f& size)
    {
        m_size.x = std::max(size.x, 0.f);
        m_size.y = std::max(size.y, 0.f);

        if (isSet())
            updateVertices();
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    Vector2f Sprite::getSize() const
    {
        return m_size;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void Sprite::setColor(const Color& color)
    {
        m_vertexColor = color;

        const sf::Color vertexColor = Color::calcColorOpacity(m_vertexColor, m_opacity);
        for (auto& vertex : m_vertices)
            vertex.color = vertexColor;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    const Color& Sprite::getColor() const
    {
        return m_vertexColor;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void Sprite::setOpacity(float opacity)
    {
        m_opacity = opacity;
        setColor(getColor());
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    float Sprite::getOpacity() const
    {
        return m_opacity;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void Sprite::setVisibleRect(const FloatRect& visibleRect)
    {
        m_visibleRect = visibleRect;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    FloatRect Sprite::getVisibleRect() const
    {
        return m_visibleRect;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    bool Sprite::isTransparentPixel(Vector2f pos) const
    {
        if (!isSet() || !m_texture.getData()->image || (m_size.x == 0) || (m_size.y == 0))
            return true;

        if (getRotation() != 0)
        {
            Vector2f offset = {getTransform().transformRect(FloatRect({}, getSize())).left,
                               getTransform().transformRect(FloatRect({}, getSize())).top};

            pos = getInverseTransform().transformPoint(pos) + getInverseTransform().transformPoint(offset);

            // Watch out for rounding errors
            const float epsilon = 0.00001f;
            if ((pos.x < 0) && (pos.x > -epsilon))
                pos.x = 0;
            if ((pos.y < 0) && (pos.y > -epsilon))
                pos.y = 0;
            if ((pos.x >= getSize().x) && (pos.x < getSize().x + epsilon))
                pos.x = getSize().x;
            if ((pos.y >= getSize().y) && (pos.y < getSize().y + epsilon))
                pos.y = getSize().y;
        }
        else // There is no rotation
            pos -= getPosition();

        if ((pos.x < 0) || (pos.y < 0) || (pos.x >= getSize().x) || (pos.y >= getSize().y))
            return true;

        // Find out on which pixel the mouse is standing
        sf::Vector2u pixel;
        sf::IntRect middleRect = m_texture.getMiddleRect();
        const sf::Texture& texture = m_texture.getData()->texture;
        switch (m_scalingType)
        {
            case ScalingType::Normal:
            {
                pixel.x = static_cast<unsigned int>(pos.x / m_size.x * texture.getSize().x);
                pixel.y = static_cast<unsigned int>(pos.y / m_size.y * texture.getSize().y);
                break;
            }
            case ScalingType::Horizontal:
            {
                if (pos.x >= m_size.x - (texture.getSize().x - middleRect.left - middleRect.width) * (m_size.y / texture.getSize().y))
                {
                    float xDiff = (pos.x - (m_size.x - (texture.getSize().x - middleRect.left - middleRect.width) * (m_size.y / texture.getSize().y)));
                    pixel.x = static_cast<unsigned int>(middleRect.left + middleRect.width + (xDiff / m_size.y * texture.getSize().y));
                }
                else if (pos.x >= middleRect.left * (m_size.y / texture.getSize().y))
                {
                    float xDiff = pos.x - (middleRect.left * (m_size.y / texture.getSize().y));
                    pixel.x = static_cast<unsigned int>(middleRect.left + (xDiff / (m_size.x - ((texture.getSize().x - middleRect.width) * (m_size.y / texture.getSize().y))) * middleRect.width));
                }
                else // Mouse on the left part
                {
                    pixel.x = static_cast<unsigned int>(pos.x / m_size.y * texture.getSize().y);
                }

                pixel.y = static_cast<unsigned int>(pos.y / m_size.y * texture.getSize().y);
                break;
            }
            case ScalingType::Vertical:
            {
                if (pos.y >= m_size.y - (texture.getSize().y - middleRect.top - middleRect.height) * (m_size.x / texture.getSize().x))
                {
                    float yDiff = (pos.y - (m_size.y - (texture.getSize().y - middleRect.top - middleRect.height) * (m_size.x / texture.getSize().x)));
                    pixel.y = static_cast<unsigned int>(middleRect.top + middleRect.height + (yDiff / m_size.x * texture.getSize().x));
                }
                else if (pos.y >= middleRect.top * (m_size.x / texture.getSize().x))
                {
                    float yDiff = pos.y - (middleRect.top * (m_size.x / texture.getSize().x));
                    pixel.y = static_cast<unsigned int>(middleRect.top + (yDiff / (m_size.y - ((texture.getSize().y - middleRect.height) * (m_size.x / texture.getSize().x))) * middleRect.height));
                }
                else // Mouse on the top part
                {
                    pixel.y = static_cast<unsigned int>(pos.y / m_size.x * texture.getSize().x);
                }

                pixel.x = static_cast<unsigned int>(pos.x / m_size.x * texture.getSize().x);
                break;
            }
            case ScalingType::NineSlice:
            {
                if (pos.x < middleRect.left)
                    pixel.x = static_cast<unsigned int>(pos.x);
                else if (pos.x >= m_size.x - (texture.getSize().x - middleRect.width - middleRect.left))
                    pixel.x = static_cast<unsigned int>(pos.x - m_size.x + texture.getSize().x);
                else
                {
                    float xDiff = (pos.x - middleRect.left) / (m_size.x - (texture.getSize().x - middleRect.width)) * middleRect.width;
                    pixel.x = static_cast<unsigned int>(middleRect.left + xDiff);
                }

                if (pos.y < middleRect.top)
                    pixel.y = static_cast<unsigned int>(pos.y);
                else if (pos.y >= m_size.y - (texture.getSize().y - middleRect.height - middleRect.top))
                    pixel.y = static_cast<unsigned int>(pos.y - m_size.y + texture.getSize().y);
                else
                {
                    float yDiff = (pos.y - middleRect.top) / (m_size.y - (texture.getSize().y - middleRect.height)) * middleRect.height;
                    pixel.y = static_cast<unsigned int>(middleRect.top + yDiff);
                }

                break;
            }
        };

        return m_texture.isTransparentPixel(pixel);
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    Sprite::ScalingType Sprite::getScalingType() const
    {
        return m_scalingType;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void Sprite::updateVertices()
    {
        // Figure out how the image is scaled best
        Vector2f textureSize{m_texture.getImageSize()};
        FloatRect middleRect{sf::FloatRect{m_texture.getMiddleRect()}};
        if (middleRect == FloatRect(0, 0, textureSize.x, textureSize.y))
        {
            m_scalingType = ScalingType::Normal;
        }
        else if (middleRect.height == textureSize.y)
        {
            if (m_size.x >= (textureSize.x - middleRect.width) * (m_size.y / textureSize.y))
                m_scalingType = ScalingType::Horizontal;
            else
                m_scalingType = ScalingType::Normal;
        }
        else if (middleRect.width == textureSize.x)
        {
            if (m_size.y >= (textureSize.y - middleRect.height) * (m_size.x / textureSize.x))
                m_scalingType = ScalingType::Vertical;
            else
                m_scalingType = ScalingType::Normal;
        }
        else
        {
            if (m_size.x >= textureSize.x - middleRect.width)
            {
                if (m_size.y >= textureSize.y - middleRect.height)
                    m_scalingType = ScalingType::NineSlice;
                else
                    m_scalingType = ScalingType::Horizontal;
            }
            else if (m_size.y >= (textureSize.y - middleRect.height) * (m_size.x / textureSize.x))
                m_scalingType = ScalingType::Vertical;
            else
                m_scalingType = ScalingType::Normal;
        }

        // Calculate the vertices based on the way we are scaling
        switch (m_scalingType)
        {
        case ScalingType::Normal:
            ///////////
            // 0---1 //
            // |   | //
            // 2---3 //
            ///////////
            m_vertices.resize(4);
            m_vertices[0] = {{0, 0}, m_vertexColor, {0, 0}};
            m_vertices[1] = {{m_size.x, 0}, m_vertexColor, {textureSize.x, 0}};
            m_vertices[2] = {{0, m_size.y}, m_vertexColor, {0, textureSize.y}};
            m_vertices[3] = {{m_size.x, m_size.y}, m_vertexColor, {textureSize.x, textureSize.y}};
            break;

        case ScalingType::Horizontal:
            ///////////////////////
            // 0---2-------4---6 //
            // |   |       |   | //
            // 1---3-------5---7 //
            ///////////////////////
            m_vertices.resize(8);
            m_vertices[0] = {{0, 0}, m_vertexColor, {0, 0}};
            m_vertices[1] = {{0, m_size.y}, m_vertexColor, {0, textureSize.y}};
            m_vertices[2] = {{middleRect.left * (m_size.y / textureSize.y), 0}, m_vertexColor, {middleRect.left, 0}};
            m_vertices[3] = {{middleRect.left * (m_size.y / textureSize.y), m_size.y}, m_vertexColor, {middleRect.left, textureSize.y}};
            m_vertices[4] = {{m_size.x - (textureSize.x - middleRect.left - middleRect.width) * (m_size.y / textureSize.y), 0}, m_vertexColor, {middleRect.left + middleRect.width, 0}};
            m_vertices[5] = {{m_size.x - (textureSize.x - middleRect.left - middleRect.width) * (m_size.y / textureSize.y), m_size.y}, m_vertexColor, {middleRect.left + middleRect.width, textureSize.y}};
            m_vertices[6] = {{m_size.x, 0}, m_vertexColor, {textureSize.x, 0}};
            m_vertices[7] = {{m_size.x, m_size.y}, m_vertexColor, {textureSize.x, textureSize.y}};
            break;

        case ScalingType::Vertical:
            ///////////
            // 0---1 //
            // |   | //
            // 2---3 //
            // |   | //
            // |   | //
            // |   | //
            // 4---5 //
            // |   | //
            // 6---7-//
            ///////////
            m_vertices.resize(8);
            m_vertices[0] = {{0, 0}, m_vertexColor, {0, 0}};
            m_vertices[1] = {{m_size.x, 0}, m_vertexColor, {textureSize.x, 0}};
            m_vertices[2] = {{0, middleRect.top * (m_size.x / textureSize.x)}, m_vertexColor, {0, middleRect.top}};
            m_vertices[3] = {{m_size.x, middleRect.top * (m_size.x / textureSize.x)}, m_vertexColor, {textureSize.x, middleRect.top}};
            m_vertices[4] = {{0, m_size.y - (textureSize.y - middleRect.top - middleRect.height) * (m_size.x / textureSize.x)}, m_vertexColor, {0, middleRect.top + middleRect.height}};
            m_vertices[5] = {{m_size.x, m_size.y - (textureSize.y - middleRect.top - middleRect.height) * (m_size.x / textureSize.x)}, m_vertexColor, {textureSize.x, middleRect.top + middleRect.height}};
            m_vertices[6] = {{0, m_size.y}, m_vertexColor, {0, textureSize.y}};
            m_vertices[7] = {{m_size.x, m_size.y}, m_vertexColor, {textureSize.x, textureSize.y}};
            break;

        case ScalingType::NineSlice:
            //////////////////////////////////
            // 0---1/13-----------14-----15 //
            // |    |              |     |  //
            // 2---3/11----------12/16---17 //
            // |    |              |     |  //
            // |    |              |     |  //
            // |    |              |     |  //
            // 4---5/9-----------10/18---19 //
            // |    |              |     |  //
            // 6----7-------------8/20---21 //
            //////////////////////////////////
            m_vertices.resize(22);
            m_vertices[0] = {{0, 0}, m_vertexColor, {0, 0}};
            m_vertices[1] = {{middleRect.left, 0}, m_vertexColor, {middleRect.left, 0}};
            m_vertices[2] = {{0, middleRect.top}, m_vertexColor, {0, middleRect.top}};
            m_vertices[3] = {{middleRect.left, middleRect.top}, m_vertexColor, {middleRect.left, middleRect.top}};
            m_vertices[4] = {{0, m_size.y - (textureSize.y - middleRect.top - middleRect.height)}, m_vertexColor, {0, middleRect.top + middleRect.height}};
            m_vertices[5] = {{middleRect.left, m_size.y - (textureSize.y - middleRect.top - middleRect.height)}, m_vertexColor, {middleRect.left, middleRect.top + middleRect.height}};
            m_vertices[6] = {{0, m_size.y}, m_vertexColor, {0, textureSize.y}};
            m_vertices[7] = {{middleRect.left, m_size.y}, m_vertexColor, {middleRect.left, textureSize.y}};
            m_vertices[8] = {{m_size.x - (textureSize.x - middleRect.left - middleRect.width), m_size.y}, m_vertexColor, {middleRect.left + middleRect.width, textureSize.y}};
            m_vertices[9] = m_vertices[5];
            m_vertices[10] = {{m_size.x - (textureSize.x - middleRect.left - middleRect.width), m_size.y - (textureSize.y - middleRect.top - middleRect.height)}, m_vertexColor, {middleRect.left + middleRect.width, middleRect.top + middleRect.height}};
            m_vertices[11] = m_vertices[3];
            m_vertices[12] = {{m_size.x - (textureSize.x - middleRect.left - middleRect.width), middleRect.top}, m_vertexColor, {middleRect.left + middleRect.width, middleRect.top}};
            m_vertices[13] = m_vertices[1];
            m_vertices[14] = {{m_size.x - (textureSize.x - middleRect.left - middleRect.width), 0}, m_vertexColor, {middleRect.left + middleRect.width, 0}};
            m_vertices[15] = {{m_size.x, 0}, m_vertexColor, {textureSize.x, 0}};
            m_vertices[16] = m_vertices[12];
            m_vertices[17] = {{m_size.x, middleRect.top}, m_vertexColor, {textureSize.x, middleRect.top}};
            m_vertices[18] = m_vertices[10];
            m_vertices[19] = {{m_size.x, m_size.y - (textureSize.y - middleRect.top - middleRect.height)}, m_vertexColor, {textureSize.x, middleRect.top + middleRect.height}};
            m_vertices[20] = m_vertices[8];
            m_vertices[21] = {{m_size.x, m_size.y}, m_vertexColor, {textureSize.x, textureSize.y}};
            break;
        };
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void Sprite::draw(sf::RenderTarget& target, sf::RenderStates states) const
    {
        // A rotation can cause the image to be shifted, so we move it upfront so that it ends at the correct location
        if (getRotation() != 0)
        {
            Vector2f pos = {getTransform().transformRect(FloatRect({}, getSize())).left,
                            getTransform().transformRect(FloatRect({}, getSize())).top};

            states.transform.translate(getPosition() - pos);
        }

        states.transform *= getTransform();

        if (isSet())
        {
            // Apply clipping when needed
        #ifdef TGUI_USE_CPP17
            std::optional<Clipping> clipping;
            if (m_visibleRect != FloatRect{})
                clipping.emplace(target, states, Vector2f{m_visibleRect.left, m_visibleRect.top}, Vector2f{m_visibleRect.width, m_visibleRect.height});
        #else
            std::unique_ptr<Clipping> clipping;
            if (m_visibleRect != FloatRect{0, 0, 0, 0})
                clipping = std::make_unique<Clipping>(target, states, Vector2f{m_visibleRect.left, m_visibleRect.top}, Vector2f{m_visibleRect.width, m_visibleRect.height});
        #endif

            states.shader = m_texture.getData()->shader;
            states.texture = &m_texture.getData()->texture;
            target.draw(m_vertices.data(), m_vertices.size(), sf::PrimitiveType::TrianglesStrip, states);
        }
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// TGUI - Texus' Graphical User Interface
// Copyright (C) 2012-2018 Bruno Van de Velde (vdv_b@tgui.eu)
//
// This software is provided 'as-is', without any express or implied warranty.
// In no event will the authors be held liable for any damages arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it freely,
// subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented;
//    you must not claim that you wrote the original software.
//    If you use this software in a product, an acknowledgment
//    in the product documentation would be appreciated but is not required.
//
// 2. Altered source versions must be plainly marked as such,
//    and must not be misrepresented as being the original software.
//
// 3. This notice may not be removed or altered from any source distribution.
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


#include <TGUI/Text.hpp>
#include <TGUI/Global.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <cmath>

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace tgui
{
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    Vector2f Text::getSize() const
    {
        return m_size;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void Text::setString(const sf::String& string)
    {
        m_text.setString(string);
        recalculateSize();
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    const sf::String& Text::getString() const
    {
        return m_text.getString();
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void Text::setCharacterSize(unsigned int size)
    {
        m_text.setCharacterSize(size);
        recalculateSize();
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    unsigned int Text::getCharacterSize() const
    {
        return m_text.getCharacterSize();
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void Text::setColor(Color color)
    {
        m_color = color;

    #if SFML_VERSION_MAJOR > 2 || (SFML_VERSION_MAJOR == 2 && SFML_VERSION_MINOR >= 4)
        m_text.setFillColor(Color::calcColorOpacity(color, m_opacity));
    #else
        m_text.setColor(Color::calcColorOpacity(color, m_opacity));
    #endif
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    Color Text::getColor() const
    {
        return m_color;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void Text::setOpacity(float opacity)
    {
        m_opacity = opacity;

    #if SFML_VERSION_MAJOR > 2 || (SFML_VERSION_MAJOR == 2 && SFML_VERSION_MINOR >= 4)
        m_text.setFillColor(Color::calcColorOpacity(m_color, opacity));
    #else
        m_text.setColor(Color::calcColorOpacity(m_color, opacity));
    #endif
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    float Text::getOpacity() const
    {
        return m_opacity;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void Text::setFont(Font font)
    {
        m_font = font;

        if (font)
            m_text.setFont(*font.getFont());
        else
        {
            // We can't keep using a pointer to the old font (it might be destroyed), but sf::Text has no function to pass an empty font
            if (m_text.getFont())
            {
                m_text = sf::Text();
                m_text.setString(getString());
                m_text.setCharacterSize(getCharacterSize());
                m_text.setStyle(getStyle());

            #if SFML_VERSION_MAJOR > 2 || (SFML_VERSION_MAJOR == 2 && SFML_VERSION_MINOR >= 4)
                m_text.setFillColor(Color::calcColorOpacity(getColor(), getOpacity()));
            #else
                m_text.setColor(Color::calcColorOpacity(getColor(), getOpacity()));
            #endif
            }
        }

        recalculateSize();
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    Font Text::getFont() const
    {
        return m_font;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void Text::setStyle(TextStyle style)
    {
        if (style != m_text.getStyle())
        {
            m_text.setStyle(style);
            recalculateSize();
        }
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    TextStyle Text::getStyle() const
    {
        return m_text.getStyle();
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    Vector2f Text::findCharacterPos(std::size_t index) const
    {
        return m_text.findCharacterPos(index);
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void Text::draw(sf::RenderTarget& target, sf::RenderStates states) const
    {
        states.transform *= getTransform();

        // Round the position to avoid blurry text
        const float* matrix = states.transform.getMatrix();
        states.transform = sf::Transform{matrix[0], matrix[4], std::round(matrix[12]),
                                         matrix[1], matrix[5], std::floor(matrix[13]),
                                         matrix[3], matrix[7], matrix[15]};

        target.draw(m_text, states);
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void Text::recalculateSize()
    {
        const std::shared_ptr<sf::Font> font = m_font;
        if (font == nullptr)
        {
            m_size = {0, 0};
            return;
        }

        float width = 0;
        float maxWidth = 0;
        unsigned int lines = 1;
        std::uint32_t prevChar = 0;
        const sf::String& string = m_text.getString();
        const bool bold = (m_text.getStyle() & sf::Text::Bold) != 0;
        const unsigned int textSize = m_text.getCharacterSize();
        for (std::size_t i = 0; i < string.getSize(); ++i)
        {
            const float kerning = font->getKerning(prevChar, string[i], textSize);
            if (string[i] == '\n')
            {
                maxWidth = std::max(maxWidth, width);
                width = 0;
                lines++;
            }
            else if (string[i] == '\t')
                width += (static_cast<float>(font->getGlyph(' ', textSize, bold).advance) * 4) + kerning;
            else
                width += static_cast<float>(font->getGlyph(string[i], textSize, bold).advance) + kerning;

            prevChar = string[i];
        }

        const float extraVerticalSpace = Text::calculateExtraVerticalSpace(m_font, m_text.getCharacterSize(), m_text.getStyle());
        const float height = lines * font->getLineSpacing(m_text.getCharacterSize()) + extraVerticalSpace;
        m_size = {std::max(maxWidth, width), height};
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    float Text::getExtraHorizontalPadding(const Text& text)
    {
        return getExtraHorizontalPadding(text.getFont(), text.getCharacterSize(), text.getStyle());
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    float Text::getExtraHorizontalPadding(Font font, unsigned int characterSize, TextStyle textStyle)
    {
        return getLineHeight(font, characterSize, textStyle) / 10.f;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    float Text::getExtraVerticalPadding(unsigned int characterSize)
    {
        return std::max(1.f, std::round(characterSize / 8.f));
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    float Text::getLineHeight(const Text& text)
    {
        return getLineHeight(text.getFont(), text.getCharacterSize(), text.getStyle());
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    float Text::getLineHeight(Font font, unsigned int characterSize, TextStyle textStyle)
    {
        const float extraVerticalSpace = Text::calculateExtraVerticalSpace(font, characterSize, textStyle);
        return font.getLineSpacing(characterSize) + extraVerticalSpace;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    unsigned int Text::findBestTextSize(Font fontWrapper, float height, int fit)
    {
        const std::shared_ptr<sf::Font> font = fontWrapper.getFont();
        if (!font)
            return 0;

        if (height < 2)
            return 1;

        std::vector<unsigned int> textSizes(static_cast<std::size_t>(height));
        for (unsigned int i = 0; i < static_cast<unsigned int>(height); ++i)
            textSizes[i] = i + 1;

        const auto high = std::lower_bound(textSizes.begin(), textSizes.end(), height,
                                           [&](unsigned int charSize, float h) { return font->getLineSpacing(charSize) + Text::calculateExtraVerticalSpace(font, charSize) < h; });
        if (high == textSizes.end())
            return static_cast<unsigned int>(height);

        const float highLineSpacing = font->getLineSpacing(*high);
        if (highLineSpacing == height)
            return *high;

        const auto low = high - 1;
        const float lowLineSpacing = font->getLineSpacing(*low);

        if (fit < 0)
            return *low;
        else if (fit > 0)
            return *high;
        else
        {
            if (std::abs(height - lowLineSpacing) < std::abs(height - highLineSpacing))
                return *low;
            else
                return *high;
        }
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    float Text::calculateExtraVerticalSpace(Font font, unsigned int characterSize, TextStyle style)
    {
        if (font == nullptr)
            return 0;

        const bool bold = (style & sf::Text::Bold) != 0;

        // Calculate the height of the first line (char size = everything above baseline, height + top = part below baseline)
        const float lineHeight = characterSize
                                 + font.getFont()->getGlyph('g', characterSize, bold).bounds.height
                                 + font.getFont()->getGlyph('g', characterSize, bold).bounds.top;

        // Get the line spacing sfml returns
        const float lineSpacing = font.getFont()->getLineSpacing(characterSize);

        // Calculate the offset of the text
        return lineHeight - lineSpacing;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    sf::String Text::wordWrap(float maxWidth, const sf::String& text, Font font, unsigned int textSize, bool bold, bool dropLeadingSpace)
    {
        if (font == nullptr)
            return "";

        sf::String result;
        std::size_t index = 0;
        while (index < text.getSize())
        {
            const std::size_t oldIndex = index;

            // Find out how many characters we can get on this line
            float width = 0;
            std::uint32_t prevChar = 0;
            for (std::size_t i = index; i < text.getSize(); ++i)
            {
                float charWidth;
                const std::uint32_t curChar = text[i];
                if (curChar == '\n')
                {
                    index++;
                    break;
                }
                else if (curChar == '\t')
                    charWidth = font.getFont()->getGlyph(' ', textSize, bold).advance * 4;
                else
                    charWidth = font.getFont()->getGlyph(curChar, textSize, bold).advance;

                const float kerning = font.getFont()->getKerning(prevChar, curChar, textSize);
                if ((maxWidth == 0) || (width + charWidth + kerning <= maxWidth))
                {
                    width += kerning + charWidth;
                    index++;
                }
                else
                    break;

                prevChar = curChar;
            }

            // Every line contains at least one character
            if (index == oldIndex)
                index++;

            // Implement the word-wrap by removing the last few characters from the line
            if (text[index-1] != '\n')
            {
                const std::size_t indexWithoutWordWrap = index;
                if ((index < text.getSize()) && (!isWhitespace(text[index])))
                {
                    std::size_t wordWrapCorrection = 0;
                    while ((index > oldIndex) && (!isWhitespace(text[index - 1])))
                    {
                        wordWrapCorrection++;
                        index--;
                    }

                    // The word can't be split but there is no other choice, it does not fit on the line
                    if ((index - oldIndex) <= wordWrapCorrection)
                        index = indexWithoutWordWrap;
                }
            }

            // If the next line starts with just a space, then the space need not be visible
            if (dropLeadingSpace)
            {
                if ((index < text.getSize()) && (text[index] == ' '))
                {
                    if ((index == 0) || (!isWhitespace(text[index-1])))
                    {
                        // But two or more spaces indicate that it is not a normal text and the spaces should not be ignored
                        if (((index + 1 < text.getSize()) && (!isWhitespace(text[index + 1]))) || (index + 1 == text.getSize()))
                            index++;
                    }
                }
            }

            result += text.substring(oldIndex, index - oldIndex);
            if ((index < text.getSize()) && (text[index-1] != '\n'))
                result += "\n";
        }

        return result;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// TGUI - Texus' Graphical User Interface
// Copyright (C) 2012-2018 Bruno Van de Velde (vdv_b@tgui.eu)
//
// This software is provided 'as-is', without any express or implied warranty.
// In no event will the authors be held liable for any damages arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it freely,
// subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented;
//    you must not claim that you wrote the original software.
//    If you use this software in a product, an acknowledgment
//    in the product documentation would be appreciated but is not required.
//
// 2. Altered source versions must be plainly marked as such,
//    and must not be misrepresented as being the original software.
//
// 3. This notice may not be removed or altered from any source distribution.
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


#include <TGUI/ToolTip.hpp>

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace tgui
{
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    sf::Time ToolTip::m_timeToDisplay = sf::milliseconds(500);
    Vector2f ToolTip::m_distanceToMouse = {5, 20};

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void ToolTip::setTimeToDisplay(const sf::Time& timeToDisplay)
    {
        m_timeToDisplay = timeToDisplay;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    sf::Time ToolTip::getTimeToDisplay()
    {
        return m_timeToDisplay;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void ToolTip::setDistanceToMouse(const Vector2f& distance)
    {
        m_distanceToMouse = distance;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    Vector2f ToolTip::getDistanceToMouse()
    {
        return m_distanceToMouse;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// TGUI - Texus' Graphical User Interface
// Copyright (C) 2012-2018 Bruno Van de Velde (vdv_b@tgui.eu)
//
// This software is provided 'as-is', without any express or implied warranty.
// In no event will the authors be held liable for any damages arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it freely,
// subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented;
//    you must not claim that you wrote the original software.
//    If you use this software in a product, an acknowledgment
//    in the product documentation would be appreciated but is not required.
//
// 2. Altered source versions must be plainly marked as such,
//    and must not be misrepresented as being the original software.
//
// 3. This notice may not be removed or altered from any source distribution.
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


#include <TGUI/Loading/Deserializer.hpp>
#include <TGUI/Loading/DataIO.hpp>
#include <TGUI/Renderers/WidgetRenderer.hpp>
#include <TGUI/Global.hpp>
#include <cstdint>
#include <cassert>

namespace tgui
{
    namespace
    {
        unsigned char hexToDec(char c)
        {
            assert((c >= '0' && c <= '9') || (c >= 'A' && c <= 'F')  || (c >= 'a' && c <= 'f'));

            if (c == 'A' || c == 'a')
                return 10;
            else if (c == 'B' || c == 'b')
                return 11;
            else if (c == 'C' || c == 'c')
                return 12;
            else if (c == 'D' || c == 'd')
                return 13;
            else if (c == 'E' || c == 'e')
                return 14;
            else if (c == 'F' || c == 'f')
                return 15;
            else // if (c >= '0' && c <= '9')
                return c - '0';
        }

        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

        bool readIntRect(std::string value, sf::IntRect& rect)
        {
            if (!value.empty() && (value[0] == '(') && (value[value.length()-1] == ')'))
            {
                std::vector<std::string> tokens = Deserializer::split(value.substr(1, value.size()-2), ',');
                if (tokens.size() == 4)
                {
                    rect = {tgui::stoi(tokens[0]), tgui::stoi(tokens[1]), tgui::stoi(tokens[2]), tgui::stoi(tokens[3])};
                    return true;
                }
            }

            return false;
        }

        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

        ObjectConverter deserializeBool(const std::string& value)
        {
            const std::string str = toLower(value);
            if (str == "true" || str == "yes" || str == "on" || str == "1")
                return {true};
            else if (str == "false" || str == "no" || str == "off" || str == "0")
                return {false};
            else
                throw Exception{"Failed to deserialize boolean from '" + str + "'"};
        }

        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

        ObjectConverter deserializeFont(const std::string& value)
        {
            if (value == "null" || value == "nullptr")
                return Font{};

            sf::String filename = Deserializer::deserialize(ObjectConverter::Type::String, value).getString();
            if (filename.isEmpty())
                return Font{};

            // Load the font but insert the resource path into the filename unless the filename is an absolute path
            auto font = std::make_shared<sf::Font>();
        #ifdef SFML_SYSTEM_WINDOWS
            if ((filename[0] != '/') && (filename[0] != '\\') && ((filename.getSize() <= 1) || (filename[1] != ':')))
        #else
            if (filename[0] != '/')
        #endif
                font->loadFromFile(getResourcePath() + filename);
            else
                font->loadFromFile(filename);

            return Font(font);
        }

        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

        ObjectConverter deserializeColor(const std::string& value)
        {
            std::string str = trim(value);

            // Make sure that the line isn't empty
            if (!str.empty())
            {
                // Check if the color is represented by a string with its name
                const auto it = Color::colorMap.find(toLower(str));
                if (it != Color::colorMap.end())
                    return it->second;

                // The color can be represented with a hexadecimal number
                if (str[0] == '#')
                {
                    // You can only have hex characters
                    for (std::size_t i = 1; i < value.length(); ++i)
                    {
                        if (!((value[i] >= '0' && value[i] <= '9') || (value[i] >= 'A' && value[i] <= 'F')  || (value[i] >= 'a' && value[i] <= 'f')))
                            throw Exception{"Failed to deserialize color '" + value + "'. Value started but '#' but contained an invalid character afterwards."};
                    }

                    // Parse the different types of strings (#123, #1234, #112233 and #11223344)
                    if (value.length() == 4)
                    {
                        return Color{static_cast<std::uint8_t>(hexToDec(value[1]) * 16 + hexToDec(value[1])),
                                     static_cast<std::uint8_t>(hexToDec(value[2]) * 16 + hexToDec(value[2])),
                                     static_cast<std::uint8_t>(hexToDec(value[3]) * 16 + hexToDec(value[3]))};
                    }
                    else if (value.length() == 5)
                    {
                        return Color{static_cast<std::uint8_t>(hexToDec(value[1]) * 16 + hexToDec(value[1])),
                                     static_cast<std::uint8_t>(hexToDec(value[2]) * 16 + hexToDec(value[2])),
                                     static_cast<std::uint8_t>(hexToDec(value[3]) * 16 + hexToDec(value[3])),
                                     static_cast<std::uint8_t>(hexToDec(value[4]) * 16 + hexToDec(value[4]))};
                    }
                    else if (value.length() == 7)
                    {
                        return Color{static_cast<std::uint8_t>(hexToDec(value[1]) * 16 + hexToDec(value[2])),
                                     static_cast<std::uint8_t>(hexToDec(value[3]) * 16 + hexToDec(value[4])),
                                     static_cast<std::uint8_t>(hexToDec(value[5]) * 16 + hexToDec(value[6]))};
                    }
                    else if (value.length() == 9)
                    {
                        return Color{static_cast<std::uint8_t>(hexToDec(value[1]) * 16 + hexToDec(value[2])),
                                     static_cast<std::uint8_t>(hexToDec(value[3]) * 16 + hexToDec(value[4])),
                                     static_cast<std::uint8_t>(hexToDec(value[5]) * 16 + hexToDec(value[6])),
                                     static_cast<std::uint8_t>(hexToDec(value[7]) * 16 + hexToDec(value[8]))};
                    }
                    else
                        throw Exception{"Failed to deserialize color '" + value + "'. Value started but '#' but has the wrong length."};
                }

                // The string can optionally start with "rgb" or "rgba", but this is ignored
                if (str.substr(0, 4) == "rgba")
                    str.erase(0, 4);
                else if (str.substr(0, 3) == "rgb")
                    str.erase(0, 3);

                // Remove the first and last characters when they are brackets
                if ((str[0] == '(') && (str[str.length()-1] == ')'))
                    str = str.substr(1, str.length()-2);

                const std::vector<std::string> tokens = Deserializer::split(str, ',');
                if (tokens.size() == 3 || tokens.size() == 4)
                {
                    return Color{static_cast<std::uint8_t>(tgui::stoi(tokens[0])),
                                 static_cast<std::uint8_t>(tgui::stoi(tokens[1])),
                                 static_cast<std::uint8_t>(tgui::stoi(tokens[2])),
                                 static_cast<std::uint8_t>((tokens.size() == 4) ? tgui::stoi(tokens[3]) : 255)};
                }
            }

            throw Exception{"Failed to deserialize color '" + value + "'."};
        }

        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

        ObjectConverter deserializeString(const std::string& value)
        {
            // Only deserialize the string when it is surrounded with quotes
            if ((value.size() >= 2) && ((value[0] == '"') && (value[value.length()-1] == '"')))
            {
                std::string result = value.substr(1, value.length()-2);

                std::size_t backslashPos = 0;
                while ((backslashPos = result.find('\\', backslashPos)) < result.size()-1)
                {
                    result.erase(backslashPos, 1);

                    if (result[backslashPos] == 'n')
                        result[backslashPos] = '\n';
                    else if (result[backslashPos] == 't')
                        result[backslashPos] = '\t';
                    else if (result[backslashPos] == 'v')
                        result[backslashPos] = '\v';

                    backslashPos++;
                }

                return {sf::String{result}};
            }
            else
                return {sf::String{value}};
        }

        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

        ObjectConverter deserializeNumber(const std::string& value)
        {
            return {tgui::stof(value)};
        }

        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

        ObjectConverter deserializeOutline(const std::string& value)
        {
            std::string str = trim(value);

            if (str.empty())
                throw Exception{"Failed to deserialize outline '" + value + "'. String was empty."};

            // Remove the brackets around the value
            if (((str.front() == '(') && (str.back() == ')')) || ((str.front() == '{') && (str.back() == '}')))
                str = str.substr(1, str.length() - 2);

            if (str.empty())
                return {Outline{0}};

            const std::vector<std::string> tokens = Deserializer::split(str, ',');
            if (tokens.size() == 1)
                return {Outline{tokens[0]}};
            else if (tokens.size() == 2)
                return {Outline{tokens[0], tokens[1]}};
            else if (tokens.size() == 4)
                return {Outline{tokens[0], tokens[1], tokens[2], tokens[3]}};
            else
                throw Exception{"Failed to deserialize outline '" + value + "'. Expected numbers separated with a comma."};
        }

        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

        ObjectConverter deserializeTexture(const std::string& value)
        {
            if (value.empty() || (toLower(value) == "none"))
                return Texture{};

            // If there are no quotes then the value just contains a filename
            if (value[0] != '"')
            {
                // Load the texture but insert the resource path into the filename unless the filename is an absolute path
            #ifdef SFML_SYSTEM_WINDOWS
                if ((value[0] != '/') && (value[0] != '\\') && ((value.size() <= 1) || (value[1] != ':')))
            #else
                if (value[0] != '/')
            #endif
                    return Texture{getResourcePath() + value};
                else
                    return Texture{value};
            }

            std::string::const_iterator c = value.begin();
            ++c; // Skip the opening quote

            std::string filename;
            char prev = '\0';

            // Look for the end quote
            bool filenameFound = false;
            while (c != value.end())
            {
                if ((*c != '"') || (prev == '\\'))
                {
                    prev = *c;
                    filename.push_back(*c);
                    ++c;
                }
                else
                {
                    ++c;
                    filenameFound = true;
                    break;
                }
            }

            if (!filenameFound)
                throw Exception{"Failed to deserialize texture '" + value + "'. Failed to find the closing quote of the filename."};

            // There may be optional parameters
            sf::IntRect partRect;
            sf::IntRect middleRect;
            bool smooth = false;

            while (removeWhitespace(value, c))
            {
                std::string word;
                auto openingBracketPos = value.find('(', c - value.begin());
                if (openingBracketPos != std::string::npos)
                    word = value.substr(c - value.begin(), openingBracketPos - (c - value.begin()));
                else
                {
                    if (toLower(trim(value.substr(c - value.begin()))) == "smooth")
                    {
                        smooth = true;
                        break;
                    }
                    else
                        throw Exception{"Failed to deserialize texture '" + value + "'. Invalid text found behind filename."};
                }

                sf::IntRect* rect = nullptr;
                if ((word == "Part") || (word == "part"))
                {
                    rect = &partRect;
                    std::advance(c, 4);
                }
                else if ((word == "Middle") || (word == "middle"))
                {
                    rect = &middleRect;
                    std::advance(c, 6);
                }
                else
                {
                    if (word.empty())
                        throw Exception{"Failed to deserialize texture '" + value + "'. Expected 'Part' or 'Middle' in front of opening bracket."};
                    else
                        throw Exception{"Failed to deserialize texture '" + value + "'. Unexpected word '" + word + "' in front of opening bracket. Expected 'Part' or 'Middle'."};
                }

                auto closeBracketPos = value.find(')', c - value.begin());
                if (closeBracketPos != std::string::npos)
                {
                    if (!readIntRect(value.substr(c - value.begin(), closeBracketPos - (c - value.begin()) + 1), *rect))
                        throw Exception{"Failed to parse " + word + " rectangle while deserializing texture '" + value + "'."};
                }
                else
                    throw Exception{"Failed to deserialize texture '" + value + "'. Failed to find closing bracket for " + word + " rectangle."};

                std::advance(c, closeBracketPos - (c - value.begin()) + 1);
            }

            return Texture{filename, partRect, middleRect, smooth};
        }

        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

        ObjectConverter deserializeTextStyle(const std::string& style)
        {
            unsigned int decodedStyle = sf::Text::Regular;
            std::vector<std::string> styles = Deserializer::split(style, '|');
            for (const auto& elem : styles)
            {
                std::string requestedStyle = toLower(elem);
                if (requestedStyle == "bold")
                    decodedStyle |= sf::Text::Bold;
                else if (requestedStyle == "italic")
                    decodedStyle |= sf::Text::Italic;
                else if (requestedStyle == "underlined")
                    decodedStyle |= sf::Text::Underlined;
                else if (requestedStyle == "strikethrough")
                    decodedStyle |= sf::Text::StrikeThrough;
            }

            return TextStyle(decodedStyle);
        }

        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

        ObjectConverter deserializeRendererData(const std::string& renderer)
        {
            std::stringstream ss{renderer};
            auto node = DataIO::parse(ss);

            // The root node should contain exactly one child which is the node we need
            if (node->propertyValuePairs.empty() && (node->children.size() == 1))
                node = std::move(node->children[0]);

            auto rendererData = RendererData::create();
            for (const auto& pair : node->propertyValuePairs)
                rendererData->propertyValuePairs[pair.first] = ObjectConverter(pair.second->value); // Did not compile with VS2015 Update 2 when using braces

            for (const auto& child : node->children)
            {
                std::stringstream ss2;
                DataIO::emit(child, ss2);
                rendererData->propertyValuePairs[toLower(child->name)] = {sf::String{"{\n" + ss2.str() + "}"}};
            }

            return rendererData;
        }

        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    std::map<ObjectConverter::Type, Deserializer::DeserializeFunc> Deserializer::m_deserializers =
        {
            {ObjectConverter::Type::Bool, deserializeBool},
            {ObjectConverter::Type::Font, deserializeFont},
            {ObjectConverter::Type::Color, deserializeColor},
            {ObjectConverter::Type::String, deserializeString},
            {ObjectConverter::Type::Number, deserializeNumber},
            {ObjectConverter::Type::Outline, deserializeOutline},
            {ObjectConverter::Type::Texture, deserializeTexture},
            {ObjectConverter::Type::TextStyle, deserializeTextStyle},
            {ObjectConverter::Type::RendererData, deserializeRendererData}
        };

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    ObjectConverter Deserializer::deserialize(ObjectConverter::Type type, const std::string& serializedString)
    {
        assert(m_deserializers.find(type) != m_deserializers.end());
        return m_deserializers[type](serializedString);
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void Deserializer::setFunction(ObjectConverter::Type type, const DeserializeFunc& deserializer)
    {
        m_deserializers[type] = deserializer;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    const Deserializer::DeserializeFunc& Deserializer::getFunction(ObjectConverter::Type type)
    {
        return m_deserializers[type];
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    std::vector<std::string> Deserializer::split(const std::string& str, char delim)
    {
        std::vector<std::string> tokens;

        std::size_t start = 0;
        std::size_t end = 0;
        while ((end = str.find(delim, start)) != std::string::npos) {
            tokens.push_back(trim(str.substr(start, end - start)));
            start = end + 1;
        }

        tokens.push_back(trim(str.substr(start)));
        return tokens;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// TGUI - Texus' Graphical User Interface
// Copyright (C) 2012-2018 Bruno Van de Velde (vdv_b@tgui.eu)
//
// This software is provided 'as-is', without any express or implied warranty.
// In no event will the authors be held liable for any damages arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it freely,
// subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented;
//    you must not claim that you wrote the original software.
//    If you use this software in a product, an acknowledgment
//    in the product documentation would be appreciated but is not required.
//
// 2. Altered source versions must be plainly marked as such,
//    and must not be misrepresented as being the original software.
//
// 3. This notice may not be removed or altered from any source distribution.
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


#include <TGUI/Loading/Deserializer.hpp>
#include <TGUI/Loading/ThemeLoader.hpp>
#include <TGUI/Loading/DataIO.hpp>
#include <TGUI/Global.hpp>

#include <cassert>
#include <sstream>
#include <fstream>
#include <set>

#ifdef SFML_SYSTEM_ANDROID
    #include <SFML/System/NativeActivity.hpp>
    #include <android/asset_manager_jni.h>
    #include <android/asset_manager.h>
    #include <android/native_activity.h>
    #include <android/configuration.h>
#endif

// Ignore warning "C4503: decorated name length exceeded, name was truncated" in Visual Studio
#if defined _MSC_VER
    #pragma warning(disable : 4503)
#endif

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace tgui
{
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    std::map<std::string, std::map<std::string, std::map<sf::String, sf::String>>> DefaultThemeLoader::m_propertiesCache;

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    namespace
    {
        // Turns texture and font filenames into paths relative to the theme file
        void injectThemePath(std::set<const DataIO::Node*>& handledSections, const std::unique_ptr<DataIO::Node>& node, const std::string& path)
        {
            for (const auto& pair : node->propertyValuePairs)
            {
                if (((pair.first.size() >= 7) && (toLower(pair.first.substr(0, 7)) == "texture")) || (pair.first == "font"))
                {
                    if (pair.second->value.isEmpty() || (pair.second->value == "null") || (pair.second->value == "nullptr"))
                        continue;

                    // Insert the path into the filename unless the filename is already an absolute path
                    if (pair.second->value[0] != '"')
                    {
                    #ifdef SFML_SYSTEM_WINDOWS
                        if ((pair.second->value[0] != '/') && (pair.second->value[0] != '\\') && ((pair.second->value.getSize() <= 1) || (pair.second->value[1] != ':')))
                    #else
                        if (pair.second->value[0] != '/')
                    #endif
                            pair.second->value = path + pair.second->value;
                    }
                    else // The filename is between quotes
                    {
                        if (pair.second->value.getSize() <= 1)
                            continue;

                    #ifdef SFML_SYSTEM_WINDOWS
                        if ((pair.second->value[1] != '/') && (pair.second->value[1] != '\\') && ((pair.second->value.getSize() <= 2) || (pair.second->value[2] != ':')))
                    #else
                        if (pair.second->value[1] != '/')
                    #endif
                            pair.second->value = '"' + path + pair.second->value.substring(1);
                    }
                }
            }

            for (const auto& child : node->children)
            {
                if (handledSections.find(child.get()) == handledSections.end())
                {
                    handledSections.insert(child.get());
                    injectThemePath(handledSections, child, path);
                }
            }
        }

        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

        void resolveReferences(std::map<std::string, std::reference_wrapper<const std::unique_ptr<DataIO::Node>>>& sections, const std::unique_ptr<DataIO::Node>& node)
        {
            for (const auto& pair : node->propertyValuePairs)
            {
                // Check if this property is a reference to another section
                if (!pair.second->value.isEmpty() && (pair.second->value[0] == '&'))
                {
                    std::string name = toLower(Deserializer::deserialize(ObjectConverter::Type::String, pair.second->value.substring(1)).getString());

                    auto sectionsIt = sections.find(name);
                    if (sectionsIt == sections.end())
                        throw Exception{"Undefined reference to '" + name + "' encountered."};

                    // Resolve references recursively
                    resolveReferences(sections, sectionsIt->second);

                    // Make a copy of the section
                    std::stringstream ss;
                    DataIO::emit(sectionsIt->second, ss);
                    pair.second->value = "{\n" + ss.str() + "}";
                }
            }

            for (const auto& child : node->children)
                resolveReferences(sections, child);
        }

        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void BaseThemeLoader::preload(const std::string&)
    {
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void DefaultThemeLoader::flushCache(const std::string& filename)
    {
        if (filename != "")
        {
            auto propertiesCacheIt = m_propertiesCache.find(filename);
            if (propertiesCacheIt != m_propertiesCache.end())
                m_propertiesCache.erase(propertiesCacheIt);
        }
        else
        {
            m_propertiesCache.clear();
        }
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void DefaultThemeLoader::preload(const std::string& filename)
    {
        if (filename == "")
            return;

        // Load the file when not already in cache
        if (m_propertiesCache.find(filename) == m_propertiesCache.end())
        {
            std::string resourcePath;
            auto slashPos = filename.find_last_of("/\\");
            if (slashPos != std::string::npos)
                resourcePath = filename.substr(0, slashPos+1);

            std::stringstream fileContents;
            readFile(filename, fileContents);

            std::unique_ptr<DataIO::Node> root = DataIO::parse(fileContents);

            if (root->propertyValuePairs.size() != 0)
                throw Exception{"Unexpected result while loading theme file '" + filename + "'. Root property-value pair found."};

            // Turn texture and font filenames into paths relative to the theme file
            if (!resourcePath.empty())
            {
                std::set<const DataIO::Node*> handledSections;
                injectThemePath(handledSections, root, resourcePath);
            }

            // Get a list of section names and map them to their nodes (needed for resolving references)
            std::map<std::string, std::reference_wrapper<const std::unique_ptr<DataIO::Node>>> sections;
            for (const auto& child : root->children)
            {
                std::string name = toLower(Deserializer::deserialize(ObjectConverter::Type::String, child->name).getString());
                sections.emplace(name, std::cref(child));
            }

            // Resolve references to sections
            resolveReferences(sections, root);

            // Cache all propery value pairs
            for (const auto& section : sections)
            {
                const auto& child = section.second;
                const std::string& name = section.first;
                for (const auto& pair : child.get()->propertyValuePairs)
                    m_propertiesCache[filename][name][toLower(pair.first)] = pair.second->value;

                for (const auto& nestedProperty : child.get()->children)
                {
                    std::stringstream ss;
                    DataIO::emit(nestedProperty, ss);
                    m_propertiesCache[filename][name][toLower(nestedProperty->name)] = "{\n" + ss.str() + "}";
                }
            }
        }
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    const std::map<sf::String, sf::String>& DefaultThemeLoader::load(const std::string& filename, const std::string& section)
    {
        preload(filename);

        const std::string lowercaseClassName = toLower(section);

        // An empty filename is not considered an error and will result in an empty property list
        if (filename.empty())
            return m_propertiesCache[""][lowercaseClassName];

        if (m_propertiesCache[filename].find(lowercaseClassName) == m_propertiesCache[filename].end())
            throw Exception{"No section '" + section + "' was found in file '" + filename + "'."};

        return m_propertiesCache[filename][lowercaseClassName];
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    bool DefaultThemeLoader::canLoad(const std::string& filename, const std::string& section)
    {
        if (filename.empty())
            return true;
        else
            return m_propertiesCache[filename].find(toLower(section)) != m_propertiesCache[filename].end();
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void DefaultThemeLoader::readFile(const std::string& filename, std::stringstream& contents) const
    {
        if (filename.empty())
            return;

        std::string fullFilename;
    #ifdef SFML_SYSTEM_WINDOWS
        if ((filename[0] != '/') && (filename[0] != '\\') && ((filename.size() <= 1) || (filename[1] != ':')))
    #else
        if (filename[0] != '/')
    #endif
            fullFilename = getResourcePath() + filename;
        else
            fullFilename = filename;

    #ifdef SFML_SYSTEM_ANDROID
        // If the file does not start with a slash then load it from the assets
        if (!fullFilename.empty() && (fullFilename[0] != '/'))
        {
            ANativeActivity* activity = sf::getNativeActivity();

            JNIEnv* env = 0;
            activity->vm->AttachCurrentThread(&env, NULL);
            jclass clazz = env->GetObjectClass(activity->clazz);

            jmethodID methodID = env->GetMethodID(clazz, "getAssets", "()Landroid/content/res/AssetManager;");
            jobject assetManagerObject = env->CallObjectMethod(activity->clazz, methodID);
            jobject globalAssetManagerRef = env->NewGlobalRef(assetManagerObject);
            AAssetManager* assetManager = AAssetManager_fromJava(env, globalAssetManagerRef);
            assert(assetManager);

            AAsset* asset = AAssetManager_open(assetManager, fullFilename.c_str(), AASSET_MODE_UNKNOWN);
            if (!asset)
                throw Exception{ "Failed to open theme file '" + fullFilename + "' from assets." };

            off_t assetLength = AAsset_getLength(asset);

            char* buffer = new char[assetLength + 1];
            AAsset_read(asset, buffer, assetLength);
            buffer[assetLength] = 0;

            contents << buffer;

            AAsset_close(asset);
            delete[] buffer;

            activity->vm->DetachCurrentThread();
        }
        else
    #endif
        {
            std::ifstream file{fullFilename};
            if (!file.is_open())
                throw Exception{"Failed to open theme file '" + fullFilename + "'."};

            contents << file.rdbuf();
            file.close();
        }
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// TGUI - Texus' Graphical User Interface
// Copyright (C) 2012-2018 Bruno Van de Velde (vdv_b@tgui.eu)
//
// This software is provided 'as-is', without any express or implied warranty.
// In no event will the authors be held liable for any damages arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it freely,
// subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented;
//    you must not claim that you wrote the original software.
//    If you use this software in a product, an acknowledgment
//    in the product documentation would be appreciated but is not required.
//
// 2. Altered source versions must be plainly marked as such,
//    and must not be misrepresented as being the original software.
//
// 3. This notice may not be removed or altered from any source distribution.
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


#include <TGUI/Renderers/ButtonRenderer.hpp>
#include <TGUI/RendererDefines.hpp>

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace tgui
{
    TGUI_RENDERER_PROPERTY_OUTLINE(ButtonRenderer, Borders)

    TGUI_RENDERER_PROPERTY_COLOR(ButtonRenderer, TextColor, Color::Black)
    TGUI_RENDERER_PROPERTY_COLOR(ButtonRenderer, TextColorHover, {})
    TGUI_RENDERER_PROPERTY_COLOR(ButtonRenderer, TextColorDown, {})
    TGUI_RENDERER_PROPERTY_COLOR(ButtonRenderer, TextColorDisabled, {})
    TGUI_RENDERER_PROPERTY_COLOR(ButtonRenderer, TextColorFocused, {})
    TGUI_RENDERER_PROPERTY_COLOR(ButtonRenderer, BackgroundColor, Color::White)
    TGUI_RENDERER_PROPERTY_COLOR(ButtonRenderer, BackgroundColorHover, {})
    TGUI_RENDERER_PROPERTY_COLOR(ButtonRenderer, BackgroundColorDown, {})
    TGUI_RENDERER_PROPERTY_COLOR(ButtonRenderer, BackgroundColorDisabled, {})
    TGUI_RENDERER_PROPERTY_COLOR(ButtonRenderer, BackgroundColorFocused, {})
    TGUI_RENDERER_PROPERTY_COLOR(ButtonRenderer, BorderColor, Color::Black)
    TGUI_RENDERER_PROPERTY_COLOR(ButtonRenderer, BorderColorHover, {})
    TGUI_RENDERER_PROPERTY_COLOR(ButtonRenderer, BorderColorDown, {})
    TGUI_RENDERER_PROPERTY_COLOR(ButtonRenderer, BorderColorDisabled, {})
    TGUI_RENDERER_PROPERTY_COLOR(ButtonRenderer, BorderColorFocused, {})

    TGUI_RENDERER_PROPERTY_TEXTURE(ButtonRenderer, Texture)
    TGUI_RENDERER_PROPERTY_TEXTURE(ButtonRenderer, TextureHover)
    TGUI_RENDERER_PROPERTY_TEXTURE(ButtonRenderer, TextureDown)
    TGUI_RENDERER_PROPERTY_TEXTURE(ButtonRenderer, TextureDisabled)
    TGUI_RENDERER_PROPERTY_TEXTURE(ButtonRenderer, TextureFocused)

    TGUI_RENDERER_PROPERTY_TEXT_STYLE(ButtonRenderer, TextStyle, sf::Text::Regular)
    TGUI_RENDERER_PROPERTY_TEXT_STYLE(ButtonRenderer, TextStyleHover, {})
    TGUI_RENDERER_PROPERTY_TEXT_STYLE(ButtonRenderer, TextStyleDown, {})
    TGUI_RENDERER_PROPERTY_TEXT_STYLE(ButtonRenderer, TextStyleDisabled, {})
    TGUI_RENDERER_PROPERTY_TEXT_STYLE(ButtonRenderer, TextStyleFocused, {})
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// TGUI - Texus' Graphical User Interface
// Copyright (C) 2012-2018 Bruno Van de Velde (vdv_b@tgui.eu)
//
// This software is provided 'as-is', without any express or implied warranty.
// In no event will the authors be held liable for any damages arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it freely,
// subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented;
//    you must not claim that you wrote the original software.
//    If you use this software in a product, an acknowledgment
//    in the product documentation would be appreciated but is not required.
//
// 2. Altered source versions must be plainly marked as such,
//    and must not be misrepresented as being the original software.
//
// 3. This notice may not be removed or altered from any source distribution.
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


#include <TGUI/Renderers/ComboBoxRenderer.hpp>
#include <TGUI/RendererDefines.hpp>

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace tgui
{
    TGUI_RENDERER_PROPERTY_OUTLINE(ComboBoxRenderer, Borders)
    TGUI_RENDERER_PROPERTY_OUTLINE(ComboBoxRenderer, Padding)

    TGUI_RENDERER_PROPERTY_COLOR(ComboBoxRenderer, BackgroundColor, Color::White)
    TGUI_RENDERER_PROPERTY_COLOR(ComboBoxRenderer, TextColor, Color::Black)
    TGUI_RENDERER_PROPERTY_COLOR(ComboBoxRenderer, ArrowBackgroundColor, Color::White)
    TGUI_RENDERER_PROPERTY_COLOR(ComboBoxRenderer, ArrowBackgroundColorHover, {})
    TGUI_RENDERER_PROPERTY_COLOR(ComboBoxRenderer, ArrowColor, Color::Black)
    TGUI_RENDERER_PROPERTY_COLOR(ComboBoxRenderer, ArrowColorHover, {})
    TGUI_RENDERER_PROPERTY_COLOR(ComboBoxRenderer, BorderColor, Color::Black)

    TGUI_RENDERER_PROPERTY_TEXTURE(ComboBoxRenderer, TextureBackground)
    TGUI_RENDERER_PROPERTY_TEXTURE(ComboBoxRenderer, TextureArrow)
    TGUI_RENDERER_PROPERTY_TEXTURE(ComboBoxRenderer, TextureArrowHover)

    TGUI_RENDERER_PROPERTY_TEXT_STYLE(ComboBoxRenderer, TextStyle, sf::Text::Regular)

    TGUI_RENDERER_PROPERTY_RENDERER(ComboBoxRenderer, ListBox, "listbox")
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// TGUI - Texus' Graphical User Interface
// Copyright (C) 2012-2018 Bruno Van de Velde (vdv_b@tgui.eu)
//
// This software is provided 'as-is', without any express or implied warranty.
// In no event will the authors be held liable for any damages arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it freely,
// subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented;
//    you must not claim that you wrote the original software.
//    If you use this software in a product, an acknowledgment
//    in the product documentation would be appreciated but is not required.
//
// 2. Altered source versions must be plainly marked as such,
//    and must not be misrepresented as being the original software.
//
// 3. This notice may not be removed or altered from any source distribution.
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


#include <TGUI/Renderers/KnobRenderer.hpp>
#include <TGUI/RendererDefines.hpp>

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace tgui
{
    TGUI_RENDERER_PROPERTY_OUTLINE(KnobRenderer, Borders)

    TGUI_RENDERER_PROPERTY_COLOR(KnobRenderer, BackgroundColor, Color::White)
    TGUI_RENDERER_PROPERTY_COLOR(KnobRenderer, ThumbColor, Color::Black)
    TGUI_RENDERER_PROPERTY_COLOR(KnobRenderer, BorderColor, Color::Black)

    TGUI_RENDERER_PROPERTY_TEXTURE(KnobRenderer, TextureBackground)
    TGUI_RENDERER_PROPERTY_TEXTURE(KnobRenderer, TextureForeground)

    TGUI_RENDERER_PROPERTY_NUMBER(KnobRenderer, ImageRotation, 0)
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// TGUI - Texus' Graphical User Interface
// Copyright (C) 2012-2018 Bruno Van de Velde (vdv_b@tgui.eu)
//
// This software is provided 'as-is', without any express or implied warranty.
// In no event will the authors be held liable for any damages arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it freely,
// subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented;
//    you must not claim that you wrote the original software.
//    If you use this software in a product, an acknowledgment
//    in the product documentation would be appreciated but is not required.
//
// 2. Altered source versions must be plainly marked as such,
//    and must not be misrepresented as being the original software.
//
// 3. This notice may not be removed or altered from any source distribution.
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


#include <TGUI/Renderers/MenuBarRenderer.hpp>
#include <TGUI/RendererDefines.hpp>

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace tgui
{
    TGUI_RENDERER_PROPERTY_COLOR(MenuBarRenderer, BackgroundColor, Color::White)
    TGUI_RENDERER_PROPERTY_COLOR(MenuBarRenderer, SelectedBackgroundColor, Color(0, 110, 255))
    TGUI_RENDERER_PROPERTY_COLOR(MenuBarRenderer, TextColor, Color::Black)
    TGUI_RENDERER_PROPERTY_COLOR(MenuBarRenderer, SelectedTextColor, Color::White)
    TGUI_RENDERER_PROPERTY_COLOR(MenuBarRenderer, TextColorDisabled, {})

    TGUI_RENDERER_PROPERTY_TEXTURE(MenuBarRenderer, TextureBackground)
    TGUI_RENDERER_PROPERTY_TEXTURE(MenuBarRenderer, TextureItemBackground)
    TGUI_RENDERER_PROPERTY_TEXTURE(MenuBarRenderer, TextureSelectedItemBackground)

    TGUI_RENDERER_PROPERTY_NUMBER(MenuBarRenderer, DistanceToSide, 0)
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// TGUI - Texus' Graphical User Interface
// Copyright (C) 2012-2018 Bruno Van de Velde (vdv_b@tgui.eu)
//
// This software is provided 'as-is', without any express or implied warranty.
// In no event will the authors be held liable for any damages arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it freely,
// subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented;
//    you must not claim that you wrote the original software.
//    If you use this software in a product, an acknowledgment
//    in the product documentation would be appreciated but is not required.
//
// 2. Altered source versions must be plainly marked as such,
//    and must not be misrepresented as being the original software.
//
// 3. This notice may not be removed or altered from any source distribution.
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


#include <TGUI/Renderers/PictureRenderer.hpp>
#include <TGUI/RendererDefines.hpp>

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace tgui
{
    TGUI_RENDERER_PROPERTY_TEXTURE(PictureRenderer, Texture)
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// TGUI - Texus' Graphical User Interface
// Copyright (C) 2012-2018 Bruno Van de Velde (vdv_b@tgui.eu)
//
// This software is provided 'as-is', without any express or implied warranty.
// In no event will the authors be held liable for any damages arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it freely,
// subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented;
//    you must not claim that you wrote the original software.
//    If you use this software in a product, an acknowledgment
//    in the product documentation would be appreciated but is not required.
//
// 2. Altered source versions must be plainly marked as such,
//    and must not be misrepresented as being the original software.
//
// 3. This notice may not be removed or altered from any source distribution.
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


#include <TGUI/Renderers/RangeSliderRenderer.hpp>
#include <TGUI/RendererDefines.hpp>

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace tgui
{
    TGUI_RENDERER_PROPERTY_COLOR(RangeSliderRenderer, SelectedTrackColor, {})
    TGUI_RENDERER_PROPERTY_COLOR(RangeSliderRenderer, SelectedTrackColorHover, {})
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// TGUI - Texus' Graphical User Interface
// Copyright (C) 2012-2018 Bruno Van de Velde (vdv_b@tgui.eu)
//
// This software is provided 'as-is', without any express or implied warranty.
// In no event will the authors be held liable for any damages arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it freely,
// subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented;
//    you must not claim that you wrote the original software.
//    If you use this software in a product, an acknowledgment
//    in the product documentation would be appreciated but is not required.
//
// 2. Altered source versions must be plainly marked as such,
//    and must not be misrepresented as being the original software.
//
// 3. This notice may not be removed or altered from any source distribution.
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


#include <TGUI/Renderers/SliderRenderer.hpp>
#include <TGUI/RendererDefines.hpp>

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace tgui
{
    TGUI_RENDERER_PROPERTY_OUTLINE(SliderRenderer, Borders)

    TGUI_RENDERER_PROPERTY_COLOR(SliderRenderer, TrackColor, Color::White)
    TGUI_RENDERER_PROPERTY_COLOR(SliderRenderer, TrackColorHover, {})
    TGUI_RENDERER_PROPERTY_COLOR(SliderRenderer, ThumbColor, Color::White)
    TGUI_RENDERER_PROPERTY_COLOR(SliderRenderer, ThumbColorHover, {})
    TGUI_RENDERER_PROPERTY_COLOR(SliderRenderer, BorderColor, Color::Black)
    TGUI_RENDERER_PROPERTY_COLOR(SliderRenderer, BorderColorHover, {})

    TGUI_RENDERER_PROPERTY_TEXTURE(SliderRenderer, TextureTrack)
    TGUI_RENDERER_PROPERTY_TEXTURE(SliderRenderer, TextureTrackHover)
    TGUI_RENDERER_PROPERTY_TEXTURE(SliderRenderer, TextureThumb)
    TGUI_RENDERER_PROPERTY_TEXTURE(SliderRenderer, TextureThumbHover)
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// TGUI - Texus' Graphical User Interface
// Copyright (C) 2012-2018 Bruno Van de Velde (vdv_b@tgui.eu)
//
// This software is provided 'as-is', without any express or implied warranty.
// In no event will the authors be held liable for any damages arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it freely,
// subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented;
//    you must not claim that you wrote the original software.
//    If you use this software in a product, an acknowledgment
//    in the product documentation would be appreciated but is not required.
//
// 2. Altered source versions must be plainly marked as such,
//    and must not be misrepresented as being the original software.
//
// 3. This notice may not be removed or altered from any source distribution.
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


#include <TGUI/Renderers/TextBoxRenderer.hpp>
#include <TGUI/RendererDefines.hpp>

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace tgui
{
    TGUI_RENDERER_PROPERTY_OUTLINE(TextBoxRenderer, Borders)
    TGUI_RENDERER_PROPERTY_OUTLINE(TextBoxRenderer, Padding)

    TGUI_RENDERER_PROPERTY_COLOR(TextBoxRenderer, BackgroundColor, Color::White)
    TGUI_RENDERER_PROPERTY_COLOR(TextBoxRenderer, TextColor, Color::Black)
    TGUI_RENDERER_PROPERTY_COLOR(TextBoxRenderer, SelectedTextColor, Color::White)
    TGUI_RENDERER_PROPERTY_COLOR(TextBoxRenderer, SelectedTextBackgroundColor, Color(0, 110, 255))
    TGUI_RENDERER_PROPERTY_COLOR(TextBoxRenderer, BorderColor, Color::Black)
    TGUI_RENDERER_PROPERTY_COLOR(TextBoxRenderer, CaretColor, Color::Black)

    TGUI_RENDERER_PROPERTY_TEXTURE(TextBoxRenderer, TextureBackground)

    TGUI_RENDERER_PROPERTY_NUMBER(TextBoxRenderer, CaretWidth, 1)

    TGUI_RENDERER_PROPERTY_RENDERER(TextBoxRenderer, Scrollbar, "scrollbar")
    TGUI_RENDERER_PROPERTY_NUMBER(TextBoxRenderer, ScrollbarWidth, 0)
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// TGUI - Texus' Graphical User Interface
// Copyright (C) 2012-2018 Bruno Van de Velde (vdv_b@tgui.eu)
//
// This software is provided 'as-is', without any express or implied warranty.
// In no event will the authors be held liable for any damages arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it freely,
// subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented;
//    you must not claim that you wrote the original software.
//    If you use this software in a product, an acknowledgment
//    in the product documentation would be appreciated but is not required.
//
// 2. Altered source versions must be plainly marked as such,
//    and must not be misrepresented as being the original software.
//
// 3. This notice may not be removed or altered from any source distribution.
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


#include <TGUI/Widgets/BitmapButton.hpp>
#include <TGUI/Clipping.hpp>

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace tgui
{
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    BitmapButton::BitmapButton()
    {
        m_type = "BitmapButton";
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    BitmapButton::Ptr BitmapButton::create()
    {
        return std::make_shared<BitmapButton>();
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    BitmapButton::Ptr BitmapButton::copy(BitmapButton::ConstPtr button)
    {
        if (button)
            return std::static_pointer_cast<BitmapButton>(button->clone());
        else
            return nullptr;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void BitmapButton::setSize(const Layout2d& size)
    {
        Button::setSize(size);

        recalculateGlyphSize();
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void BitmapButton::setText(const sf::String& text)
    {
        m_string = text;
        m_text.setString(text);
        m_text.setCharacterSize(m_textSize);

        if (m_autoSize)
            updateSize();
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void BitmapButton::setImage(const Texture& image)
    {
        m_glyphTexture = image;
        m_glyphSprite.setTexture(m_glyphTexture);
        updateSize();
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    const Texture& BitmapButton::getImage() const
    {
        return m_glyphTexture;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void BitmapButton::setImageScaling(float relativeHeight)
    {
        m_relativeGlyphHeight = relativeHeight;
        updateSize();
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    float BitmapButton::getImageScaling() const
    {
        return m_relativeGlyphHeight;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    std::unique_ptr<DataIO::Node> BitmapButton::save(SavingRenderersMap& renderers) const
    {
        auto node = Button::save(renderers);
        node->propertyValuePairs["Image"] = std::make_unique<DataIO::ValueNode>(Serializer::serialize(m_glyphTexture));
        node->propertyValuePairs["ImageScaling"] = std::make_unique<DataIO::ValueNode>(Serializer::serialize(m_relativeGlyphHeight));
        return node;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void BitmapButton::load(const std::unique_ptr<DataIO::Node>& node, const LoadingRenderersMap& renderers)
    {
        Button::load(node, renderers);

        if (node->propertyValuePairs["image"])
            setImage(Deserializer::deserialize(ObjectConverter::Type::Texture, node->propertyValuePairs["image"]->value).getTexture());
        if (node->propertyValuePairs["imagescaling"])
            setImageScaling(Deserializer::deserialize(ObjectConverter::Type::Number, node->propertyValuePairs["imagescaling"]->value).getNumber());
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void BitmapButton::draw(sf::RenderTarget& target, sf::RenderStates states) const
    {
        if (!m_glyphSprite.isSet())
        {
            Button::draw(target, states);
            return;
        }

        states.transform.translate(getPosition());

        // Draw the borders
        if (m_bordersCached != Borders{0})
        {
            drawBorders(target, states, m_bordersCached, getSize(), getCurrentBorderColor());
            states.transform.translate(m_bordersCached.getOffset());
        }

        // Check if there is a background texture
        if (m_sprite.isSet())
        {
            if (!m_enabled && m_spriteDisabled.isSet())
                m_spriteDisabled.draw(target, states);
            else if (m_mouseHover)
            {
                if (m_mouseDown && m_spriteDown.isSet())
                    m_spriteDown.draw(target, states);
                else if (m_spriteHover.isSet())
                    m_spriteHover.draw(target, states);
                else
                    m_sprite.draw(target, states);
            }
            else
                m_sprite.draw(target, states);

            // When the button is focused then draw an extra image
            if (m_focused && m_spriteFocused.isSet())
                m_spriteFocused.draw(target, states);
        }
        else // There is no background texture
        {
            drawRectangleShape(target, states, getInnerSize(), getCurrentBackgroundColor());
        }

        Clipping clipping(target, states, {}, getInnerSize());
        if (m_text.getString().isEmpty())
        {
            states.transform.translate({(getInnerSize().x - m_glyphSprite.getSize().x) / 2.f, (getInnerSize().y - m_glyphSprite.getSize().y) / 2.f});
            m_glyphSprite.draw(target, states);
        }
        else // There is some text next to the glyph
        {
            const float distanceBetweenTextAndImage = m_text.getSize().y / 5.f;
            const float width = m_glyphSprite.getSize().x + distanceBetweenTextAndImage + m_text.getSize().x;
            states.transform.translate({(getInnerSize().x - width) / 2.f, (getInnerSize().y - m_glyphSprite.getSize().y) / 2.f});
            m_glyphSprite.draw(target, states);
            states.transform.translate({m_glyphSprite.getSize().x + distanceBetweenTextAndImage, (m_glyphSprite.getSize().y - m_text.getSize().y) / 2.f});
            m_text.draw(target, states);
        }
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void BitmapButton::updateSize()
    {
        if (!m_glyphSprite.isSet())
        {
            Button::updateSize();
            return;
        }

        if (m_autoSize)
        {
            Widget::setSize({getSize().x, m_text.getSize().y * 1.25f + m_bordersCached.getTop() + m_bordersCached.getBottom()});

            recalculateGlyphSize();

            if (m_text.getString().isEmpty())
            {
                Widget::setSize({m_glyphSprite.getSize().x + (getInnerSize().y - m_glyphSprite.getSize().y) + m_bordersCached.getLeft() + m_bordersCached.getRight(),
                                 getSize().y});
            }
            else
            {
                const float spaceAroundImageAndText = m_text.getSize().y;
                const float distanceBetweenTextAndImage = m_text.getSize().y / 5.f;
                Widget::setSize({m_glyphSprite.getSize().x + distanceBetweenTextAndImage + m_text.getSize().x
                                + spaceAroundImageAndText + m_bordersCached.getLeft() + m_bordersCached.getRight(), getSize().y});
            }
        }
        else
            recalculateGlyphSize();

        m_bordersCached.updateParentSize(getSize());

        // Reset the texture sizes
        m_sprite.setSize(getInnerSize());
        m_spriteHover.setSize(getInnerSize());
        m_spriteDown.setSize(getInnerSize());
        m_spriteDisabled.setSize(getInnerSize());
        m_spriteFocused.setSize(getInnerSize());
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void BitmapButton::recalculateGlyphSize()
    {
        if (!m_glyphSprite.isSet())
            return;

        if (m_relativeGlyphHeight == 0)
            m_glyphSprite.setSize(m_glyphTexture.getImageSize());
        else
            m_glyphSprite.setSize({m_relativeGlyphHeight * getInnerSize().y,
                                   (m_relativeGlyphHeight * getInnerSize().y) / m_glyphTexture.getImageSize().y * m_glyphTexture.getImageSize().x});
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// TGUI - Texus' Graphical User Interface
// Copyright (C) 2012-2018 Bruno Van de Velde (vdv_b@tgui.eu)
//
// This software is provided 'as-is', without any express or implied warranty.
// In no event will the authors be held liable for any damages arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it freely,
// subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented;
//    you must not claim that you wrote the original software.
//    If you use this software in a product, an acknowledgment
//    in the product documentation would be appreciated but is not required.
//
// 2. Altered source versions must be plainly marked as such,
//    and must not be misrepresented as being the original software.
//
// 3. This notice may not be removed or altered from any source distribution.
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


#include <TGUI/Widgets/Button.hpp>

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace tgui
{
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    Button::Button()
    {
        m_type = "Button";
        m_text.setFont(m_fontCached);

        m_renderer = aurora::makeCopied<ButtonRenderer>();
        setRenderer(Theme::getDefault()->getRendererNoThrow(m_type));

        setTextSize(getGlobalTextSize());
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    Button::Ptr Button::create(const sf::String& text)
    {
        auto button = std::make_shared<Button>();

        if (!text.isEmpty())
            button->setText(text);

        return button;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    Button::Ptr Button::copy(Button::ConstPtr button)
    {
        if (button)
            return std::static_pointer_cast<Button>(button->clone());
        else
            return nullptr;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    ButtonRenderer* Button::getSharedRenderer()
    {
        return aurora::downcast<ButtonRenderer*>(Widget::getSharedRenderer());
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    const ButtonRenderer* Button::getSharedRenderer() const
    {
        return aurora::downcast<const ButtonRenderer*>(Widget::getSharedRenderer());
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    ButtonRenderer* Button::getRenderer()
    {
        return aurora::downcast<ButtonRenderer*>(Widget::getRenderer());
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    const ButtonRenderer* Button::getRenderer() const
    {
        return aurora::downcast<const ButtonRenderer*>(Widget::getRenderer());
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void Button::setSize(const Layout2d& size)
    {
        Widget::setSize(size);

        m_autoSize = false;
        updateSize();
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void Button::setEnabled(bool enabled)
    {
        Widget::setEnabled(enabled);
        updateTextColorAndStyle();
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void Button::setText(const sf::String& text)
    {
        m_string = text;
        m_text.setString(text);

        // Set the text size when the text has a fixed size
        if (m_textSize != 0)
        {
            m_text.setCharacterSize(m_textSize);
            updateSize();
        }

        if ((getInnerSize().x < 0) || (getInnerSize().y < 0))
            return;

        // Draw the text normally unless the height is more than double of the width
        if (getInnerSize().y <= getInnerSize().x * 2)
        {
            // Auto size the text when necessary
            if (m_textSize == 0)
            {
                const unsigned int textSize = Text::findBestTextSize(m_fontCached, getInnerSize().y * 0.8f);
                m_text.setCharacterSize(textSize);

                // Make the text smaller when it's too width
                if (m_text.getSize().x > (getInnerSize().x * 0.85f))
                    m_text.setCharacterSize(static_cast<unsigned int>(textSize * ((getInnerSize().x * 0.85f) / m_text.getSize().x)));
            }
        }
        else // Place the text vertically
        {
            // The text is vertical
            if (!m_string.isEmpty())
            {
                m_text.setString(m_string[0]);

                for (unsigned int i = 1; i < m_string.getSize(); ++i)
                    m_text.setString(m_text.getString() + "\n" + m_string[i]);
            }

            // Auto size the text when necessary
            if (m_textSize == 0)
            {
                unsigned int textSize = Text::findBestTextSize(m_fontCached, getInnerSize().x * 0.8f);
                m_text.setCharacterSize(textSize);

                // Make the text smaller when it's too high
                if (m_text.getSize().y > (getInnerSize().y * 0.85f))
                    m_text.setCharacterSize(static_cast<unsigned int>(textSize * getInnerSize().y * 0.85f / m_text.getSize().y));
            }
        }
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    const sf::String& Button::getText() const
    {
        return m_string;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void Button::setTextSize(unsigned int size)
    {
        if (size != m_textSize)
        {
            m_textSize = size;

            // Call setText to reposition the text
            setText(getText());
        }
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    unsigned int Button::getTextSize() const
    {
        return m_text.getCharacterSize();
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    bool Button::mouseOnWidget(Vector2f pos) const
    {
        if (ClickableWidget::mouseOnWidget(pos))
        {
            if (!m_transparentTextureCached || !m_sprite.isTransparentPixel(pos - getPosition() - m_bordersCached.getOffset()))
                return true;
        }

        return false;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void Button::leftMousePressed(Vector2f pos)
    {
        ClickableWidget::leftMousePressed(pos);

        updateTextColorAndStyle();
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void Button::leftMouseReleased(Vector2f pos)
    {
        if (m_mouseDown)
            onPress.emit(this, m_text.getString());

        ClickableWidget::leftMouseReleased(pos);

        updateTextColorAndStyle();
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void Button::keyPressed(const sf::Event::KeyEvent& event)
    {
        if ((event.code == sf::Keyboard::Space) || (event.code == sf::Keyboard::Return))
            onPress.emit(this, m_text.getString());
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void Button::mouseEnteredWidget()
    {
        Widget::mouseEnteredWidget();
        updateTextColorAndStyle();
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void Button::mouseLeftWidget()
    {
        Widget::mouseLeftWidget();
        updateTextColorAndStyle();
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    Signal& Button::getSignal(std::string signalName)
    {
        if (signalName == toLower(onPress.getName()))
            return onPress;
        else
            return ClickableWidget::getSignal(std::move(signalName));
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void Button::rendererChanged(const std::string& property)
    {
        if (property == "borders")
        {
            m_bordersCached = getSharedRenderer()->getBorders();
            updateSize();
        }
        else if ((property == "textcolor") || (property == "textcolorhover") || (property == "textcolordown") || (property == "textcolordisabled") || (property == "textcolorfocused")
              || (property == "textstyle") || (property == "textstylehover") || (property == "textstyledown") || (property == "textstyledisabled") || (property == "textstylefocused"))
        {
            updateTextColorAndStyle();
        }
        else if (property == "texture")
        {
            m_sprite.setTexture(getSharedRenderer()->getTexture());
        }
        else if (property == "texturehover")
        {
            m_spriteHover.setTexture(getSharedRenderer()->getTextureHover());
        }
        else if (property == "texturedown")
        {
            m_spriteDown.setTexture(getSharedRenderer()->getTextureDown());
        }
        else if (property == "texturedisabled")
        {
            m_spriteDisabled.setTexture(getSharedRenderer()->getTextureDisabled());
        }
        else if (property == "texturefocused")
        {
            m_spriteFocused.setTexture(getSharedRenderer()->getTextureFocused());
        }
        else if (property == "bordercolor")
        {
            m_borderColorCached = getSharedRenderer()->getBorderColor();
        }
        else if (property == "bordercolorhover")
        {
            m_borderColorHoverCached = getSharedRenderer()->getBorderColorHover();
        }
        else if (property == "bordercolordown")
        {
            m_borderColorDownCached = getSharedRenderer()->getBorderColorDown();
        }
        else if (property == "bordercolordisabled")
        {
            m_borderColorDisabledCached = getSharedRenderer()->getBorderColorDisabled();
        }
        else if (property == "bordercolorfocused")
        {
            m_borderColorFocusedCached = getSharedRenderer()->getBorderColorFocused();
        }
        else if (property == "backgroundcolor")
        {
            m_backgroundColorCached = getSharedRenderer()->getBackgroundColor();
        }
        else if (property == "backgroundcolorhover")
        {
            m_backgroundColorHoverCached = getSharedRenderer()->getBackgroundColorHover();
        }
        else if (property == "backgroundcolordown")
        {
            m_backgroundColorDownCached = getSharedRenderer()->getBackgroundColorDown();
        }
        else if (property == "backgroundcolordisabled")
        {
            m_backgroundColorDisabledCached = getSharedRenderer()->getBackgroundColorDisabled();
        }
        else if (property == "backgroundcolorfocused")
        {
            m_backgroundColorFocusedCached = getSharedRenderer()->getBackgroundColorFocused();
        }
        else if (property == "opacity")
        {
            Widget::rendererChanged(property);

            m_sprite.setOpacity(m_opacityCached);
            m_spriteHover.setOpacity(m_opacityCached);
            m_spriteDown.setOpacity(m_opacityCached);
            m_spriteDisabled.setOpacity(m_opacityCached);
            m_spriteFocused.setOpacity(m_opacityCached);

            m_text.setOpacity(m_opacityCached);
        }
        else if (property == "font")
        {
            Widget::rendererChanged(property);

            m_text.setFont(m_fontCached);
            setText(getText());
        }
        else
            Widget::rendererChanged(property);
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    std::unique_ptr<DataIO::Node> Button::save(SavingRenderersMap& renderers) const
    {
        auto node = Widget::save(renderers);

        if (!m_string.isEmpty())
            node->propertyValuePairs["Text"] = std::make_unique<DataIO::ValueNode>(Serializer::serialize(m_string));
        if (m_textSize > 0)
            node->propertyValuePairs["TextSize"] = std::make_unique<DataIO::ValueNode>(to_string(m_textSize));

        return node;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void Button::load(const std::unique_ptr<DataIO::Node>& node, const LoadingRenderersMap& renderers)
    {
        Widget::load(node, renderers);

        if (node->propertyValuePairs["text"])
            setText(Deserializer::deserialize(ObjectConverter::Type::String, node->propertyValuePairs["text"]->value).getString());
        if (node->propertyValuePairs["textsize"])
            setTextSize(tgui::stoi(node->propertyValuePairs["textsize"]->value));
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    Vector2f Button::getInnerSize() const
    {
        return {getSize().x - m_bordersCached.getLeft() - m_bordersCached.getRight(),
                getSize().y - m_bordersCached.getTop() - m_bordersCached.getBottom()};
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    const Color& Button::getCurrentBackgroundColor() const
    {
        if (!m_enabled && m_backgroundColorDisabledCached.isSet())
            return m_backgroundColorDisabledCached;
        else if (m_mouseHover && m_mouseDown && m_backgroundColorDownCached.isSet())
            return m_backgroundColorDownCached;
        else if (m_mouseHover && m_backgroundColorHoverCached.isSet())
            return m_backgroundColorHoverCached;
        else if (m_focused && m_backgroundColorFocusedCached.isSet())
            return m_backgroundColorFocusedCached;
        else
            return m_backgroundColorCached;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    const Color& Button::getCurrentBorderColor() const
    {
        if (!m_enabled && m_borderColorDisabledCached.isSet())
            return m_borderColorDisabledCached;
        else if (m_mouseHover && m_mouseDown && m_borderColorDownCached.isSet())
            return m_borderColorDownCached;
        else if (m_mouseHover && m_borderColorHoverCached.isSet())
            return m_borderColorHoverCached;
        else if (m_focused && m_borderColorFocusedCached.isSet())
            return m_borderColorFocusedCached;
        else
            return m_borderColorCached;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void Button::updateTextColorAndStyle()
    {
        if (!m_enabled && getSharedRenderer()->getTextStyleDisabled().isSet())
            m_text.setStyle(getSharedRenderer()->getTextStyleDisabled());
        else if (m_mouseHover && m_mouseDown && getSharedRenderer()->getTextStyleDown().isSet())
            m_text.setStyle(getSharedRenderer()->getTextStyleDown());
        else if (m_mouseHover && getSharedRenderer()->getTextStyleHover().isSet())
            m_text.setStyle(getSharedRenderer()->getTextStyleHover());
        else if (m_focused && getSharedRenderer()->getTextStyleFocused().isSet())
            m_text.setStyle(getSharedRenderer()->getTextStyleFocused());
        else
            m_text.setStyle(getSharedRenderer()->getTextStyle());

        if (!m_enabled && getSharedRenderer()->getTextColorDisabled().isSet())
            m_text.setColor(getSharedRenderer()->getTextColorDisabled());
        else if (m_mouseHover && m_mouseDown && getSharedRenderer()->getTextColorDown().isSet())
            m_text.setColor(getSharedRenderer()->getTextColorDown());
        else if (m_mouseHover && getSharedRenderer()->getTextColorHover().isSet())
            m_text.setColor(getSharedRenderer()->getTextColorHover());
        else if (m_focused && getSharedRenderer()->getTextColorFocused().isSet())
            m_text.setColor(getSharedRenderer()->getTextColorFocused());
        else
            m_text.setColor(getSharedRenderer()->getTextColor());
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void Button::updateSize()
    {
        if (m_autoSize)
        {
            const float spaceAroundText = m_text.getSize().y;
            Widget::setSize({m_text.getSize().x + spaceAroundText + m_bordersCached.getLeft() + m_bordersCached.getRight(),
                             m_text.getSize().y * 1.25f + m_bordersCached.getTop() + m_bordersCached.getBottom()});
        }

        m_bordersCached.updateParentSize(getSize());

        // Reset the texture sizes
        m_sprite.setSize(getInnerSize());
        m_spriteHover.setSize(getInnerSize());
        m_spriteDown.setSize(getInnerSize());
        m_spriteDisabled.setSize(getInnerSize());
        m_spriteFocused.setSize(getInnerSize());

        // Recalculate the text size when auto sizing
        if (m_textSize == 0)
            setText(getText());
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void Button::draw(sf::RenderTarget& target, sf::RenderStates states) const
    {
        states.transform.translate(getPosition());

        // Draw the borders
        if (m_bordersCached != Borders{0})
        {
            drawBorders(target, states, m_bordersCached, getSize(), getCurrentBorderColor());
            states.transform.translate(m_bordersCached.getOffset());
        }

        // Check if there is a background texture
        if (m_sprite.isSet())
        {
            if (!m_enabled && m_spriteDisabled.isSet())
                m_spriteDisabled.draw(target, states);
            else if (m_mouseHover && m_mouseDown && m_spriteDown.isSet())
                m_spriteDown.draw(target, states);
            else if (m_mouseHover && m_spriteHover.isSet())
                m_spriteHover.draw(target, states);
            else if (m_focused && m_spriteFocused.isSet())
                m_spriteFocused.draw(target, states);
            else
                m_sprite.draw(target, states);
        }
        else // There is no background texture
        {
            drawRectangleShape(target, states, getInnerSize(), getCurrentBackgroundColor());
        }

        // If the button has a text then also draw the text
        states.transform.translate({(getInnerSize().x - m_text.getSize().x) / 2.f, (getInnerSize().y - m_text.getSize().y) / 2.f});
        m_text.draw(target, states);
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// TGUI - Texus' Graphical User Interface
// Copyright (C) 2012-2018 Bruno Van de Velde (vdv_b@tgui.eu)
//
// This software is provided 'as-is', without any express or implied warranty.
// In no event will the authors be held liable for any damages arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it freely,
// subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented;
//    you must not claim that you wrote the original software.
//    If you use this software in a product, an acknowledgment
//    in the product documentation would be appreciated but is not required.
//
// 2. Altered source versions must be plainly marked as such,
//    and must not be misrepresented as being the original software.
//
// 3. This notice may not be removed or altered from any source distribution.
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


#include <TGUI/Widgets/CheckBox.hpp>
#include <TGUI/Clipping.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <cmath>

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace tgui
{
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    CheckBox::CheckBox()
    {
        m_type = "CheckBox";

        setRenderer(Theme::getDefault()->getRendererNoThrow(m_type));

        setSize({Text::getLineHeight(m_text) + m_bordersCached.getLeft() + m_bordersCached.getRight(),
                 Text::getLineHeight(m_text) + m_bordersCached.getTop() + m_bordersCached.getBottom()});
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    CheckBox::Ptr CheckBox::create(sf::String text)
    {
        auto checkBox = std::make_shared<CheckBox>();

        if (!text.isEmpty())
            checkBox->setText(text);

        return checkBox;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    CheckBox::Ptr CheckBox::copy(CheckBox::ConstPtr checkbox)
    {
        if (checkbox)
            return std::static_pointer_cast<CheckBox>(checkbox->clone());
        else
            return nullptr;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    Vector2f CheckBox::getFullSize() const
    {
        if (m_spriteUnchecked.isSet() && m_spriteChecked.isSet()
         && (m_textureUncheckedCached.getImageSize() != m_textureCheckedCached.getImageSize()))
        {
            Vector2f sizeDiff = m_spriteChecked.getSize() - m_spriteUnchecked.getSize();
            if (getText().isEmpty())
                return getSize() + Vector2f{std::max(0.f, sizeDiff.x - m_bordersCached.getRight()), std::max(0.f, sizeDiff.y - m_bordersCached.getTop())};
            else
                return getSize() + Vector2f{(getSize().x * m_textDistanceRatioCached) + m_text.getSize().x, std::max(0.f, std::max((m_text.getSize().y - getSize().y) / 2, sizeDiff.y - m_bordersCached.getTop()))};
        }
        else
        {
            if (getText().isEmpty())
                return getSize();
            else
                return {getSize().x + (getSize().x * m_textDistanceRatioCached) + m_text.getSize().x, std::max(getSize().y, m_text.getSize().y)};
        }
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    Vector2f CheckBox::getWidgetOffset() const
    {
        float yOffset = 0;
        if (m_spriteUnchecked.isSet() && m_spriteChecked.isSet()
         && (m_textureUncheckedCached.getImageSize() != m_textureCheckedCached.getImageSize()))
        {
            const float sizeDiff = m_spriteChecked.getSize().y - m_spriteUnchecked.getSize().y;
            if (sizeDiff > m_bordersCached.getTop())
                yOffset = sizeDiff - m_bordersCached.getTop();
        }

        if (getText().isEmpty() || (getSize().y >= m_text.getSize().y))
            return {0, -yOffset};
        else
            return {0, -std::max(yOffset, (m_text.getSize().y - getSize().y) / 2)};
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void CheckBox::setChecked(bool checked)
    {
        if (checked && !m_checked)
        {
            m_checked = true;

            updateTextColor();
            if (m_textStyleCheckedCached.isSet())
                m_text.setStyle(m_textStyleCheckedCached);
            else
                m_text.setStyle(m_textStyleCached);

            onCheck.emit(this, true);
        }
        else
            RadioButton::setChecked(checked);
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void CheckBox::leftMouseReleased(Vector2f pos)
    {
        const bool mouseDown = m_mouseDown;

        ClickableWidget::leftMouseReleased(pos);

        // Check or uncheck when we clicked on the checkbox (not just mouse release)
        if (mouseDown)
            setChecked(!m_checked);
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void CheckBox::keyPressed(const sf::Event::KeyEvent& event)
    {
        // Check or uncheck the checkbox if the space key or the return key was pressed
        if ((event.code == sf::Keyboard::Space) || (event.code == sf::Keyboard::Return))
            setChecked(!m_checked);
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void CheckBox::rendererChanged(const std::string& property)
    {
        if (property == "textureunchecked")
            m_textureUncheckedCached = getSharedRenderer()->getTextureUnchecked();
        else if (property == "texturechecked")
            m_textureCheckedCached = getSharedRenderer()->getTextureChecked();

        RadioButton::rendererChanged(property);
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void CheckBox::updateTextureSizes()
    {
        if (m_spriteUnchecked.isSet() && m_spriteChecked.isSet())
        {
            m_spriteUnchecked.setSize(getInnerSize());
            m_spriteChecked.setSize(
                {getInnerSize().x + ((m_textureCheckedCached.getImageSize().x - m_textureUncheckedCached.getImageSize().x) * (getInnerSize().x / m_textureUncheckedCached.getImageSize().x)),
                 getInnerSize().y + ((m_textureCheckedCached.getImageSize().y - m_textureUncheckedCached.getImageSize().y) * (getInnerSize().y / m_textureUncheckedCached.getImageSize().y))}
            );

            m_spriteUncheckedHover.setSize(m_spriteUnchecked.getSize());
            m_spriteCheckedHover.setSize(m_spriteChecked.getSize());

            m_spriteUncheckedDisabled.setSize(m_spriteUnchecked.getSize());
            m_spriteCheckedDisabled.setSize(m_spriteChecked.getSize());

            m_spriteUncheckedFocused.setSize(m_spriteUnchecked.getSize());
            m_spriteCheckedFocused.setSize(m_spriteChecked.getSize());
        }
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void CheckBox::draw(sf::RenderTarget& target, sf::RenderStates states) const
    {
        states.transform.translate(getPosition());

        // Draw the borders
        if (m_bordersCached != Borders{0})
            drawBorders(target, states, m_bordersCached, getSize(), getCurrentBorderColor());

        states.transform.translate({m_bordersCached.getLeft(), m_bordersCached.getTop()});
        if (m_spriteUnchecked.isSet() && m_spriteChecked.isSet())
        {
            if (m_checked)
            {
                const Sprite* checkedSprite;
                if (!m_enabled && m_spriteCheckedDisabled.isSet())
                    checkedSprite = &m_spriteCheckedDisabled;
                else if (m_mouseHover && m_spriteCheckedHover.isSet())
                    checkedSprite = &m_spriteCheckedHover;
                else if (m_focused && m_spriteCheckedFocused.isSet())
                    checkedSprite = &m_spriteCheckedFocused;
                else
                    checkedSprite = &m_spriteChecked;

                // The image may need to be shifted when the check leaves the box
                if (getInnerSize().y != checkedSprite->getSize().y)
                {
                    float diff = getInnerSize().y - checkedSprite->getSize().y;

                    states.transform.translate({0, diff});
                    checkedSprite->draw(target, states);
                    states.transform.translate({0, -diff});
                }
                else // Draw the checked texture normally
                    checkedSprite->draw(target, states);
            }
            else
            {
                if (!m_enabled && m_spriteUncheckedDisabled.isSet())
                    m_spriteUncheckedDisabled.draw(target, states);
                else if (m_mouseHover && m_spriteUncheckedHover.isSet())
                    m_spriteUncheckedHover.draw(target, states);
                else if (m_focused && m_spriteUncheckedFocused.isSet())
                    m_spriteUncheckedFocused.draw(target, states);
                else
                    m_spriteUnchecked.draw(target, states);
            }
        }
        else // There are no images
        {
            drawRectangleShape(target, states, getInnerSize(), getCurrentBackgroundColor());

            if (m_checked)
            {
                const float pi = 3.14159265358979f;
                const Color& checkColor = getCurrentCheckColor();
                const Vector2f size = getInnerSize();
                const float lineThickness = std::min(size.x, size.y) / 5;
                const Vector2f leftPoint = {0.14f * size.x, 0.4f * size.y};
                const Vector2f middlePoint = {0.44f * size.x, 0.7f * size.y};
                const Vector2f rightPoint = {0.86f * size.x, 0.28f * size.y};
                const float x = (lineThickness / 2.f) * std::cos(pi / 4.f);
                const float y = (lineThickness / 2.f) * std::sin(pi / 4.f);
                const std::vector<sf::Vertex> vertices = {
                    {{leftPoint.x - x, leftPoint.y + y}, checkColor},
                    {{leftPoint.x + x, leftPoint.y - y}, checkColor},
                    {{middlePoint.x, middlePoint.y + 2*y}, checkColor},
                    {{middlePoint.x, middlePoint.y - 2*y}, checkColor},
                    {{rightPoint.x + x, rightPoint.y + y}, checkColor},
                    {{rightPoint.x - x, rightPoint.y - y}, checkColor}
                };

                target.draw(vertices.data(), vertices.size(), sf::PrimitiveType::TrianglesStrip, states);
            }
        }
        states.transform.translate({-m_bordersCached.getLeft(), -m_bordersCached.getTop()});

        if (!getText().isEmpty())
        {
            states.transform.translate({(1 + m_textDistanceRatioCached) * getSize().x, (getSize().y - m_text.getSize().y) / 2.0f});
            m_text.draw(target, states);
        }
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// TGUI - Texus' Graphical User Interface
// Copyright (C) 2012-2018 Bruno Van de Velde (vdv_b@tgui.eu)
//
// This software is provided 'as-is', without any express or implied warranty.
// In no event will the authors be held liable for any damages arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it freely,
// subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented;
//    you must not claim that you wrote the original software.
//    If you use this software in a product, an acknowledgment
//    in the product documentation would be appreciated but is not required.
//
// 2. Altered source versions must be plainly marked as such,
//    and must not be misrepresented as being the original software.
//
// 3. This notice may not be removed or altered from any source distribution.
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


#include <TGUI/Container.hpp>
#include <TGUI/Widgets/ComboBox.hpp>
#include <TGUI/Clipping.hpp>
#include <SFML/Graphics/ConvexShape.hpp>
#include <TGUI/SignalImpl.hpp>

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace tgui
{
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    ComboBox::ComboBox()
    {
        m_type = "ComboBox";
        m_draggableWidget = true;
        m_text.setFont(m_fontCached);

        initListBox();

        m_renderer = aurora::makeCopied<ComboBoxRenderer>();
        setRenderer(Theme::getDefault()->getRendererNoThrow(m_type));

        setTextSize(getGlobalTextSize());
        setSize({Text::getLineHeight(m_text) * 10,
                 Text::getLineHeight(m_text) * 1.25f + m_paddingCached.getTop() + m_paddingCached.getBottom() + m_bordersCached.getTop() + m_bordersCached.getBottom()});
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    ComboBox::ComboBox(const ComboBox& other) :
        Widget                           {other},
        onItemSelect                     {other.onItemSelect},
        m_nrOfItemsToDisplay             {other.m_nrOfItemsToDisplay},
        m_listBox                        {ListBox::copy(other.m_listBox)},
        m_text                           {other.m_text},
        m_expandDirection                {other.m_expandDirection},
        m_spriteBackground               {other.m_spriteBackground},
        m_spriteArrow                    {other.m_spriteArrow},
        m_spriteArrowHover               {other.m_spriteArrowHover},
        m_bordersCached                  {other.m_bordersCached},
        m_paddingCached                  {other.m_paddingCached},
        m_borderColorCached              {other.m_borderColorCached},
        m_backgroundColorCached          {other.m_backgroundColorCached},
        m_arrowColorCached               {other.m_arrowColorCached},
        m_arrowColorHoverCached          {other.m_arrowColorHoverCached},
        m_arrowBackgroundColorCached     {other.m_arrowBackgroundColorCached},
        m_arrowBackgroundColorHoverCached{other.m_arrowBackgroundColorHoverCached}
    {
        initListBox();
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    ComboBox::ComboBox(ComboBox&& other) :
        Widget                           {std::move(other)},
        onItemSelect                     {std::move(other.onItemSelect)},
        m_nrOfItemsToDisplay             {std::move(other.m_nrOfItemsToDisplay)},
        m_listBox                        {std::move(other.m_listBox)},
        m_text                           {std::move(other.m_text)},
        m_expandDirection                {std::move(other.m_expandDirection)},
        m_spriteBackground               {std::move(other.m_spriteBackground)},
        m_spriteArrow                    {std::move(other.m_spriteArrow)},
        m_spriteArrowHover               {std::move(other.m_spriteArrowHover)},
        m_bordersCached                  {std::move(other.m_bordersCached)},
        m_paddingCached                  {std::move(other.m_paddingCached)},
        m_borderColorCached              {std::move(other.m_borderColorCached)},
        m_backgroundColorCached          {std::move(other.m_backgroundColorCached)},
        m_arrowColorCached               {std::move(other.m_arrowColorCached)},
        m_arrowColorHoverCached          {std::move(other.m_arrowColorHoverCached)},
        m_arrowBackgroundColorCached     {std::move(other.m_arrowBackgroundColorCached)},
        m_arrowBackgroundColorHoverCached{std::move(other.m_arrowBackgroundColorHoverCached)}
    {
        initListBox();
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    ComboBox& ComboBox::operator= (const ComboBox& other)
    {
        if (this != &other)
        {
            ComboBox temp{other};
            Widget::operator=(other);

            std::swap(onItemSelect,                      temp.onItemSelect);
            std::swap(m_nrOfItemsToDisplay,              temp.m_nrOfItemsToDisplay);
            std::swap(m_listBox,                         temp.m_listBox);
            std::swap(m_text,                            temp.m_text);
            std::swap(m_expandDirection,                 temp.m_expandDirection);
            std::swap(m_spriteBackground,                temp.m_spriteBackground);
            std::swap(m_spriteArrow,                     temp.m_spriteArrow);
            std::swap(m_spriteArrowHover,                temp.m_spriteArrowHover);
            std::swap(m_bordersCached,                   temp.m_bordersCached);
            std::swap(m_paddingCached,                   temp.m_paddingCached);
            std::swap(m_borderColorCached,               temp.m_borderColorCached);
            std::swap(m_backgroundColorCached,           temp.m_backgroundColorCached);
            std::swap(m_arrowColorCached,                temp.m_arrowColorCached);
            std::swap(m_arrowColorHoverCached,           temp.m_arrowColorHoverCached);
            std::swap(m_arrowBackgroundColorCached,      temp.m_arrowBackgroundColorCached);
            std::swap(m_arrowBackgroundColorHoverCached, temp.m_arrowBackgroundColorHoverCached);
        }

        return *this;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    ComboBox& ComboBox::operator= (ComboBox&& other)
    {
        if (this != &other)
        {
            Widget::operator=(std::move(other));
            onItemSelect                      = std::move(other.onItemSelect);
            m_nrOfItemsToDisplay              = std::move(other.m_nrOfItemsToDisplay);
            m_listBox                         = std::move(other.m_listBox);
            m_text                            = std::move(other.m_text);
            m_expandDirection                 = std::move(other.m_expandDirection);
            m_spriteBackground                = std::move(other.m_spriteBackground);
            m_spriteArrow                     = std::move(other.m_spriteArrow);
            m_spriteArrowHover                = std::move(other.m_spriteArrowHover);
            m_bordersCached                   = std::move(other.m_bordersCached);
            m_paddingCached                   = std::move(other.m_paddingCached);
            m_borderColorCached               = std::move(other.m_borderColorCached);
            m_backgroundColorCached           = std::move(other.m_backgroundColorCached);
            m_arrowColorCached                = std::move(other.m_arrowColorCached);
            m_arrowColorHoverCached           = std::move(other.m_arrowColorHoverCached);
            m_arrowBackgroundColorCached      = std::move(other.m_arrowBackgroundColorCached);
            m_arrowBackgroundColorHoverCached = std::move(other.m_arrowBackgroundColorHoverCached);
        }

        return *this;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    ComboBox::Ptr ComboBox::create()
    {
        return std::make_shared<ComboBox>();
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    ComboBox::Ptr ComboBox::copy(ComboBox::ConstPtr comboBox)
    {
        if (comboBox)
            return std::static_pointer_cast<ComboBox>(comboBox->clone());
        else
            return nullptr;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    ComboBoxRenderer* ComboBox::getSharedRenderer()
    {
        return aurora::downcast<ComboBoxRenderer*>(Widget::getSharedRenderer());
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    const ComboBoxRenderer* ComboBox::getSharedRenderer() const
    {
        return aurora::downcast<const ComboBoxRenderer*>(Widget::getSharedRenderer());
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    ComboBoxRenderer* ComboBox::getRenderer()
    {
        return aurora::downcast<ComboBoxRenderer*>(Widget::getRenderer());
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    const ComboBoxRenderer* ComboBox::getRenderer() const
    {
        return aurora::downcast<const ComboBoxRenderer*>(Widget::getRenderer());
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void ComboBox::setSize(const Layout2d& size)
    {
        Widget::setSize(size);

        m_bordersCached.updateParentSize(getSize());
        m_paddingCached.updateParentSize(getSize());

        m_spriteBackground.setSize(getInnerSize());

        const float height = getInnerSize().y - m_paddingCached.getTop() - m_paddingCached.getBottom();

        if (height > 0)
        {
            m_listBox->setItemHeight(static_cast<unsigned int>(height));
            updateListBoxHeight();
        }

        if (m_spriteArrow.isSet())
        {
            m_spriteArrow.setSize({m_spriteArrow.getTexture().getImageSize().x * (height / m_spriteArrow.getTexture().getImageSize().y), height});
            m_spriteArrowHover.setSize(m_spriteArrow.getSize());
        }

        m_text.setCharacterSize(m_listBox->getTextSize());
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void ComboBox::setItemsToDisplay(std::size_t nrOfItemsInList)
    {
        m_nrOfItemsToDisplay = nrOfItemsInList;

        if (m_nrOfItemsToDisplay < m_listBox->getItemCount())
            updateListBoxHeight();
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    std::size_t ComboBox::getItemCount() const
    {
        return m_listBox->getItemCount();
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    std::size_t ComboBox::getItemsToDisplay() const
    {
        return m_nrOfItemsToDisplay;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    bool ComboBox::addItem(const sf::String& item, const sf::String& id)
    {
        const bool ret = m_listBox->addItem(item, id);
        updateListBoxHeight();
        return ret;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    bool ComboBox::setSelectedItem(const sf::String& itemName)
    {
        const bool ret = m_listBox->setSelectedItem(itemName);
        m_text.setString(m_listBox->getSelectedItem());
        return ret;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    bool ComboBox::setSelectedItemById(const sf::String& id)
    {
        const bool ret = m_listBox->setSelectedItemById(id);
        m_text.setString(m_listBox->getSelectedItem());
        return ret;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    bool ComboBox::setSelectedItemByIndex(std::size_t index)
    {
        const bool ret = m_listBox->setSelectedItemByIndex(index);
        m_text.setString(m_listBox->getSelectedItem());
        return ret;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void ComboBox::deselectItem()
    {
        m_text.setString("");
        m_listBox->deselectItem();
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    bool ComboBox::removeItem(const sf::String& itemName)
    {
        // Implemented like removeItemByIndex instead of like removeItemById because m_listBox->getItems() may be slow

        const bool ret = m_listBox->removeItem(itemName);

        m_text.setString(m_listBox->getSelectedItem());

        // Shrink the list size
        if ((m_nrOfItemsToDisplay == 0) || (m_listBox->getItemCount() < m_nrOfItemsToDisplay))
            updateListBoxHeight();

        return ret;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    bool ComboBox::removeItemById(const sf::String& id)
    {
        const auto& ids = m_listBox->getItemIds();
        for (std::size_t i = 0; i < ids.size(); ++i)
        {
            if (ids[i] == id)
                return removeItemByIndex(i);
        }

        return false;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    bool ComboBox::removeItemByIndex(std::size_t index)
    {
        const bool ret = m_listBox->removeItemByIndex(index);

        m_text.setString(m_listBox->getSelectedItem());

        // Shrink the list size
        if ((m_nrOfItemsToDisplay == 0) || (m_listBox->getItemCount() < m_nrOfItemsToDisplay))
            updateListBoxHeight();

        return ret;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void ComboBox::removeAllItems()
    {
        m_text.setString("");
        m_listBox->removeAllItems();

        updateListBoxHeight();
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    sf::String ComboBox::getItemById(const sf::String& id) const
    {
        return m_listBox->getItemById(id);
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    sf::String ComboBox::getSelectedItem() const
    {
        return m_listBox->getSelectedItem();
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    sf::String ComboBox::getSelectedItemId() const
    {
        return m_listBox->getSelectedItemId();
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    int ComboBox::getSelectedItemIndex() const
    {
        return m_listBox->getSelectedItemIndex();
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    bool ComboBox::changeItem(const sf::String& originalValue, const sf::String& newValue)
    {
        const bool ret = m_listBox->changeItem(originalValue, newValue);
        m_text.setString(m_listBox->getSelectedItem());
        return ret;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    bool ComboBox::changeItemById(const sf::String& id, const sf::String& newValue)
    {
        const bool ret = m_listBox->changeItemById(id, newValue);
        m_text.setString(m_listBox->getSelectedItem());
        return ret;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    bool ComboBox::changeItemByIndex(std::size_t index, const sf::String& newValue)
    {
        const bool ret = m_listBox->changeItemByIndex(index, newValue);
        m_text.setString(m_listBox->getSelectedItem());
        return ret;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    std::vector<sf::String> ComboBox::getItems() const
    {
        return m_listBox->getItems();
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    const std::vector<sf::String>& ComboBox::getItemIds() const
    {
        return m_listBox->getItemIds();
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void ComboBox::setMaximumItems(std::size_t maximumItems)
    {
        m_listBox->setMaximumItems(maximumItems);
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    std::size_t ComboBox::getMaximumItems() const
    {
        return m_listBox->getMaximumItems();
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void ComboBox::setTextSize(unsigned int textSize)
    {
        m_listBox->setTextSize(textSize);
        m_text.setCharacterSize(m_listBox->getTextSize());
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    unsigned int ComboBox::getTextSize() const
    {
        return m_listBox->getTextSize();
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void ComboBox::setExpandDirection(ExpandDirection direction)
    {
        m_expandDirection = direction;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    ComboBox::ExpandDirection ComboBox::getExpandDirection() const
    {
        return m_expandDirection;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    bool ComboBox::contains(const sf::String& item) const
    {
        const auto& items = getItems();
        return std::find(items.begin(), items.end(), item) != items.end();
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    bool ComboBox::containsId(const sf::String& id) const
    {
        const auto& ids = getItemIds();
        return std::find(ids.begin(), ids.end(), id) != ids.end();
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void ComboBox::setParent(Container* parent)
    {
        hideListBox();
        Widget::setParent(parent);
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    bool ComboBox::mouseOnWidget(Vector2f pos) const
    {
        if (FloatRect{getPosition().x, getPosition().y, getSize().x, getSize().y}.contains(pos))
        {
            if (!m_transparentTextureCached || !m_spriteBackground.isTransparentPixel(pos - getPosition() - m_bordersCached.getOffset()))
                return true;
        }

        return false;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void ComboBox::leftMousePressed(Vector2f)
    {
        m_mouseDown = true;

        // If the list wasn't visible then open it
        if (!m_listBox->isVisible())
        {
            // Show the list
            showListBox();

            // Reselect the selected item to make sure it is always among the visible items when the list opens
            if (m_listBox->getSelectedItemIndex() >= 0)
                m_listBox->setSelectedItemByIndex(m_listBox->getSelectedItemIndex());
        }
        else // This list was already open, so close it now
            hideListBox();
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    bool ComboBox::mouseWheelScrolled(float delta, Vector2f)
    {
        // Only act to scrolling when the list is not being shown
        if (m_listBox->isVisible())
            return false;

        // Check if you are scrolling down
        if (delta < 0)
        {
            // Select the next item
            if (static_cast<std::size_t>(m_listBox->getSelectedItemIndex() + 1) < m_listBox->getItemCount())
            {
                m_listBox->setSelectedItemByIndex(static_cast<std::size_t>(m_listBox->getSelectedItemIndex() + 1));
                m_text.setString(m_listBox->getSelectedItem());
            }
        }
        else // You are scrolling up
        {
            // Select the previous item
            if (m_listBox->getSelectedItemIndex() > 0)
            {
                m_listBox->setSelectedItemByIndex(static_cast<std::size_t>(m_listBox->getSelectedItemIndex() - 1));
                m_text.setString(m_listBox->getSelectedItem());
            }
        }

        return true;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    Signal& ComboBox::getSignal(std::string signalName)
    {
        if (signalName == toLower(onItemSelect.getName()))
            return onItemSelect;
        else
            return Widget::getSignal(std::move(signalName));
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void ComboBox::rendererChanged(const std::string& property)
    {
        if (property == "borders")
        {
            m_bordersCached = getSharedRenderer()->getBorders();
            setSize(m_size);
        }
        else if (property == "padding")
        {
            m_paddingCached = getSharedRenderer()->getPadding();
            setSize(m_size);
        }
        else if (property == "textcolor")
        {
            m_text.setColor(getSharedRenderer()->getTextColor());
        }
        else if (property == "textstyle")
        {
            m_text.setStyle(getSharedRenderer()->getTextStyle());
        }
        else if (property == "texturebackground")
        {
            m_spriteBackground.setTexture(getSharedRenderer()->getTextureBackground());
        }
        else if (property == "texturearrow")
        {
            m_spriteArrow.setTexture(getSharedRenderer()->getTextureArrow());
            setSize(m_size);
        }
        else if (property == "texturearrowhover")
        {
            m_spriteArrowHover.setTexture(getSharedRenderer()->getTextureArrowHover());
        }
        else if (property == "listbox")
        {
            m_listBox->setRenderer(getSharedRenderer()->getListBox());
        }
        else if (property == "bordercolor")
        {
            m_borderColorCached = getSharedRenderer()->getBorderColor();
        }
        else if (property == "backgroundcolor")
        {
            m_backgroundColorCached = getSharedRenderer()->getBackgroundColor();
        }
        else if (property == "arrowbackgroundcolor")
        {
            m_arrowBackgroundColorCached = getSharedRenderer()->getArrowBackgroundColor();
        }
        else if (property == "arrowbackgroundcolorhover")
        {
            m_arrowBackgroundColorHoverCached = getSharedRenderer()->getArrowBackgroundColorHover();
        }
        else if (property == "arrowcolor")
        {
            m_arrowColorCached = getSharedRenderer()->getArrowColor();
        }
        else if (property == "arrowcolorhover")
        {
            m_arrowColorHoverCached = getSharedRenderer()->getArrowColorHover();
        }
        else if (property == "opacity")
        {
            Widget::rendererChanged(property);

            m_spriteBackground.setOpacity(m_opacityCached);
            m_spriteArrow.setOpacity(m_opacityCached);
            m_spriteArrowHover.setOpacity(m_opacityCached);

            m_text.setOpacity(m_opacityCached);
        }
        else if (property == "font")
        {
            Widget::rendererChanged(property);

            m_text.setFont(m_fontCached);
            m_listBox->setInheritedFont(m_fontCached);

            setSize(m_size);
        }
        else
            Widget::rendererChanged(property);
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    std::unique_ptr<DataIO::Node> ComboBox::save(SavingRenderersMap& renderers) const
    {
        auto node = Widget::save(renderers);

        if (getItemCount() > 0)
        {
            const auto& items = getItems();
            const auto& ids = getItemIds();

            bool itemIdsUsed = false;
            std::string itemList = "[" + Serializer::serialize(items[0]);
            std::string itemIdList = "[" + Serializer::serialize(ids[0]);
            for (std::size_t i = 1; i < items.size(); ++i)
            {
                itemList += ", " + Serializer::serialize(items[i]);
                itemIdList += ", " + Serializer::serialize(ids[i]);

                if (!ids[i].isEmpty())
                    itemIdsUsed = true;
            }
            itemList += "]";
            itemIdList += "]";

            node->propertyValuePairs["Items"] = std::make_unique<DataIO::ValueNode>(itemList);
            if (itemIdsUsed)
                node->propertyValuePairs["ItemIds"] = std::make_unique<DataIO::ValueNode>(itemIdList);
        }

        node->propertyValuePairs["ItemsToDisplay"] = std::make_unique<DataIO::ValueNode>(to_string(getItemsToDisplay()));
        node->propertyValuePairs["TextSize"] = std::make_unique<DataIO::ValueNode>(to_string(getTextSize()));
        node->propertyValuePairs["MaximumItems"] = std::make_unique<DataIO::ValueNode>(to_string(getMaximumItems()));

        if (getExpandDirection() != ComboBox::ExpandDirection::Down)
            node->propertyValuePairs["ExpandDirection"] = std::make_unique<DataIO::ValueNode>("Up");

        return node;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void ComboBox::load(const std::unique_ptr<DataIO::Node>& node, const LoadingRenderersMap& renderers)
    {
        Widget::load(node, renderers);

        if (node->propertyValuePairs["items"])
        {
            if (!node->propertyValuePairs["items"]->listNode)
                throw Exception{"Failed to parse 'Items' property, expected a list as value"};

            if (node->propertyValuePairs["itemids"])
            {
                if (!node->propertyValuePairs["itemids"]->listNode)
                    throw Exception{"Failed to parse 'ItemIds' property, expected a list as value"};

                if (node->propertyValuePairs["items"]->valueList.size() != node->propertyValuePairs["itemids"]->valueList.size())
                    throw Exception{"Amounts of values for 'Items' differs from the amount in 'ItemIds'"};

                for (std::size_t i = 0; i < node->propertyValuePairs["items"]->valueList.size(); ++i)
                {
                    addItem(Deserializer::deserialize(ObjectConverter::Type::String, node->propertyValuePairs["items"]->valueList[i]).getString(),
                            Deserializer::deserialize(ObjectConverter::Type::String, node->propertyValuePairs["itemids"]->valueList[i]).getString());
                }
            }
            else // There are no item ids
            {
                for (const auto& item : node->propertyValuePairs["items"]->valueList)
                    addItem(Deserializer::deserialize(ObjectConverter::Type::String, item).getString());
            }
        }
        else // If there are no items, there should be no item ids
        {
            if (node->propertyValuePairs["itemids"])
                throw Exception{"Found 'ItemIds' property while there is no 'Items' property"};
        }

        if (node->propertyValuePairs["itemstodisplay"])
            setItemsToDisplay(tgui::stoi(node->propertyValuePairs["itemstodisplay"]->value));
        if (node->propertyValuePairs["textsize"])
            setTextSize(tgui::stoi(node->propertyValuePairs["textsize"]->value));
        if (node->propertyValuePairs["maximumitems"])
            setMaximumItems(tgui::stoi(node->propertyValuePairs["maximumitems"]->value));

        if (node->propertyValuePairs["expanddirection"])
        {
            if (toLower(node->propertyValuePairs["expanddirection"]->value) == "up")
                setExpandDirection(ComboBox::ExpandDirection::Up);
            else if (toLower(node->propertyValuePairs["expanddirection"]->value) == "down")
                setExpandDirection(ComboBox::ExpandDirection::Down);
            else
                throw Exception{"Failed to parse ExpandDirection property. Only the values Up and Down are correct."};
        }
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    Vector2f ComboBox::getInnerSize() const
    {
        return {getSize().x - m_bordersCached.getLeft() - m_bordersCached.getRight(),
                getSize().y - m_bordersCached.getTop() - m_bordersCached.getBottom()};
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void ComboBox::updateListBoxHeight()
    {
        const Borders borders = m_listBox->getSharedRenderer()->getBorders();
        const Padding padding = m_listBox->getSharedRenderer()->getPadding();

        if (m_nrOfItemsToDisplay > 0)
            m_listBox->setSize({getSize().x, (m_listBox->getItemHeight() * (std::min<std::size_t>(m_nrOfItemsToDisplay, std::max<std::size_t>(m_listBox->getItemCount(), 1))))
                                             + borders.getTop() + borders.getBottom() + padding.getTop() + padding.getBottom()});
        else
            m_listBox->setSize({getSize().x, (m_listBox->getItemHeight() * std::max<std::size_t>(m_listBox->getItemCount(), 1))
                                             + borders.getTop() + borders.getBottom() + padding.getTop() + padding.getBottom()});

    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void ComboBox::showListBox()
    {
        if (!m_listBox->isVisible() && getParent())
        {
            m_listBox->setVisible(true);

            // Find the GuiContainer that contains the combo box
            Container* container = getParent();
            while (container->getParent() != nullptr)
                container = container->getParent();

            if (m_expandDirection == ExpandDirection::Down)
                m_listBox->setPosition({getAbsolutePosition().x, getAbsolutePosition().y + getSize().y - m_bordersCached.getBottom()});
            else // if (m_expandDirection == ExpandDirection::Up)
                m_listBox->setPosition({getAbsolutePosition().x, getAbsolutePosition().y - m_listBox->getSize().y + m_bordersCached.getTop()});

            container->add(m_listBox, "#TGUI_INTERNAL$ComboBoxListBox#");
            m_listBox->setFocused(true);
        }
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void ComboBox::hideListBox()
    {
        // If the list was open then close it now
        if (m_listBox->isVisible())
        {
            m_listBox->setVisible(false);
            m_listBox->mouseNoLongerOnWidget();

            // Find the GuiContainer in order to remove the ListBox from it
            Widget* container = this;
            while (container->getParent() != nullptr)
                container = container->getParent();

            if (container != this)
                static_cast<Container*>(container)->remove(m_listBox);
        }
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void ComboBox::initListBox()
    {
        m_listBox->setVisible(false);

        m_listBox->connect("ItemSelected", [this](){
                                                m_text.setString(m_listBox->getSelectedItem());
                                                onItemSelect.emit(this, m_listBox->getSelectedItem(), m_listBox->getSelectedItemId());
                                            });

        m_listBox->connect("Unfocused", [this](){
                                                    if (!m_mouseHover)
                                                        hideListBox();
                                                });

        m_listBox->connect("MouseReleased", [this](){ hideListBox(); });
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void ComboBox::draw(sf::RenderTarget& target, sf::RenderStates states) const
    {
        states.transform.translate(getPosition());

        // Draw the borders
        if (m_bordersCached != Borders{0})
        {
            drawBorders(target, states, m_bordersCached, getSize(), m_borderColorCached);
            states.transform.translate(m_bordersCached.getOffset());
        }

        sf::RenderStates statesForText = states;

        // Draw the background
        if (m_spriteBackground.isSet())
            m_spriteBackground.draw(target, states);
        else
            drawRectangleShape(target, states, getInnerSize(), m_backgroundColorCached);

        // Check if we have a texture for the arrow
        float arrowSize;
        if (m_spriteArrow.isSet())
        {
            arrowSize = m_spriteArrow.getSize().x;
            states.transform.translate({getInnerSize().x - m_paddingCached.getRight() - arrowSize, m_paddingCached.getTop()});

            if (m_mouseHover && m_spriteArrowHover.isSet())
                m_spriteArrowHover.draw(target, states);
            else
                m_spriteArrow.draw(target, states);
        }
        else // There are no textures for the arrow
        {
            arrowSize = getInnerSize().y - m_paddingCached.getTop() - m_paddingCached.getBottom();
            states.transform.translate({getInnerSize().x - m_paddingCached.getRight() - arrowSize, m_paddingCached.getTop()});

            if (m_mouseHover && m_arrowBackgroundColorHoverCached.isSet())
                drawRectangleShape(target, states, {arrowSize, arrowSize}, m_arrowBackgroundColorHoverCached);
            else
                drawRectangleShape(target, states, {arrowSize, arrowSize}, m_arrowBackgroundColorCached);

            sf::ConvexShape arrow{3};
            arrow.setPoint(0, {arrowSize / 5, arrowSize / 5});
            arrow.setPoint(1, {arrowSize / 2, arrowSize * 4/5});
            arrow.setPoint(2, {arrowSize * 4/5, arrowSize / 5});

            if (m_mouseHover && m_arrowColorHoverCached.isSet())
                arrow.setFillColor(m_arrowColorHoverCached);
            else
                arrow.setFillColor(m_arrowColorCached);

            target.draw(arrow, states);
        }

        // Draw the selected item
        if (!m_text.getString().isEmpty())
        {
            const Clipping clipping{target, statesForText, {m_paddingCached.getLeft(), m_paddingCached.getTop()}, {getInnerSize().x - m_paddingCached.getLeft() - m_paddingCached.getRight() - arrowSize, getInnerSize().y - m_paddingCached.getTop() - m_paddingCached.getBottom()}};

            statesForText.transform.translate(m_paddingCached.getLeft() + Text::getExtraHorizontalPadding(m_text),
                                              m_paddingCached.getTop() + (((getInnerSize().y - m_paddingCached.getTop() - m_paddingCached.getBottom()) - m_text.getSize().y) / 2.0f));
            m_text.draw(target, statesForText);
        }
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// TGUI - Texus' Graphical User Interface
// Copyright (C) 2012-2018 Bruno Van de Velde (vdv_b@tgui.eu)
//
// This software is provided 'as-is', without any express or implied warranty.
// In no event will the authors be held liable for any damages arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it freely,
// subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented;
//    you must not claim that you wrote the original software.
//    If you use this software in a product, an acknowledgment
//    in the product documentation would be appreciated but is not required.
//
// 2. Altered source versions must be plainly marked as such,
//    and must not be misrepresented as being the original software.
//
// 3. This notice may not be removed or altered from any source distribution.
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


#include <TGUI/Widgets/Grid.hpp>
#include <TGUI/SignalImpl.hpp>

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace tgui
{
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    Grid::Grid()
    {
        m_type = "Grid";
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    Grid::Grid(const Grid& gridToCopy) :
        Container {gridToCopy},
        m_autoSize{gridToCopy.m_autoSize}
    {
        for (std::size_t row = 0; row < gridToCopy.m_gridWidgets.size(); ++row)
        {
            for (std::size_t col = 0; col < gridToCopy.m_gridWidgets[row].size(); ++col)
            {
                // Find the widget that belongs in this square
                for (std::size_t i = 0; i < gridToCopy.m_widgets.size(); ++i)
                {
                    // If a widget matches then add it to the grid
                    if (gridToCopy.m_widgets[i] == gridToCopy.m_gridWidgets[row][col])
                        addWidget(m_widgets[i], row, col, gridToCopy.m_objPadding[row][col], gridToCopy.m_objAlignment[row][col]);
                }
            }
        }
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    Grid::Grid(Grid&& gridToMove) :
        Container           {std::move(gridToMove)},
        m_autoSize          {std::move(gridToMove.m_autoSize)},
        m_gridWidgets       {std::move(gridToMove.m_gridWidgets)},
        m_objPadding        {std::move(gridToMove.m_objPadding)},
        m_objAlignment      {std::move(gridToMove.m_objAlignment)},
        m_rowHeight         {std::move(gridToMove.m_rowHeight)},
        m_columnWidth       {std::move(gridToMove.m_columnWidth)},
        m_connectedCallbacks{}
    {
        for (auto& widget : m_widgets)
        {
            widget->disconnect(gridToMove.m_connectedCallbacks[widget]);
            m_connectedCallbacks[widget] = widget->connect("SizeChanged", [this](){ updateWidgets(); });
        }
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    Grid& Grid::operator= (const Grid& right)
    {
        // Make sure it is not the same widget
        if (this != &right)
        {
            Container::operator=(right);
            m_autoSize = right.m_autoSize;
            m_connectedCallbacks.clear();

            for (std::size_t row = 0; row < right.m_gridWidgets.size(); ++row)
            {
                for (std::size_t col = 0; col < right.m_gridWidgets[row].size(); ++col)
                {
                    // Find the widget that belongs in this square
                    for (std::size_t i = 0; i < right.m_widgets.size(); ++i)
                    {
                        // If a widget matches then add it to the grid
                        if (right.m_widgets[i] == right.m_gridWidgets[row][col])
                            addWidget(m_widgets[i], row, col, right.m_objPadding[row][col], right.m_objAlignment[row][col]);
                    }
                }
            }
        }

        return *this;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    Grid& Grid::operator= (Grid&& right)
    {
        if (this != &right)
        {
            Container::operator=(std::move(right));
            m_autoSize           = std::move(right.m_autoSize);
            m_gridWidgets        = std::move(right.m_gridWidgets);
            m_objPadding         = std::move(right.m_objPadding);
            m_objAlignment       = std::move(right.m_objAlignment);
            m_rowHeight          = std::move(right.m_rowHeight);
            m_columnWidth        = std::move(right.m_columnWidth);
            m_connectedCallbacks = std::move(right.m_connectedCallbacks);

            for (auto& widget : m_widgets)
            {
                widget->disconnect(right.m_connectedCallbacks[widget]);
                m_connectedCallbacks[widget] = widget->connect("SizeChanged", [this](){ updateWidgets(); });
            }
        }

        return *this;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    Grid::Ptr Grid::create()
    {
        return std::make_shared<Grid>();
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    Grid::Ptr Grid::copy(Grid::ConstPtr grid)
    {
        if (grid)
            return std::static_pointer_cast<Grid>(grid->clone());
        else
            return nullptr;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void Grid::setSize(const Layout2d& size)
    {
        Container::setSize(size);

        m_autoSize = false;

        updatePositionsOfAllWidgets();
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void Grid::setAutoSize(bool autoSize)
    {
        if (m_autoSize != autoSize)
        {
            m_autoSize = autoSize;
            updatePositionsOfAllWidgets();
        }
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    bool Grid::getAutoSize() const
    {
        return m_autoSize;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    bool Grid::remove(const Widget::Ptr& widget)
    {
        const auto callbackIt = m_connectedCallbacks.find(widget);
        if (callbackIt != m_connectedCallbacks.end())
        {
            widget->disconnect(callbackIt->second);
            m_connectedCallbacks.erase(callbackIt);
        }

        // Find the widget in the grid
        for (std::size_t row = 0; row < m_gridWidgets.size(); ++row)
        {
            for (std::size_t col = 0; col < m_gridWidgets[row].size(); ++col)
            {
                if (m_gridWidgets[row][col] == widget)
                {
                    // Remove the widget from the grid
                    m_gridWidgets[row].erase(m_gridWidgets[row].begin() + col);
                    m_objPadding[row].erase(m_objPadding[row].begin() + col);
                    m_objAlignment[row].erase(m_objAlignment[row].begin() + col);

                    // Check if this is the last column
                    if (m_columnWidth.size() == m_gridWidgets[row].size() + 1)
                    {
                        // Check if there is another row with this many columns
                        bool rowFound = false;
                        for (std::size_t i = 0; i < m_gridWidgets.size(); ++i)
                        {
                            if (m_gridWidgets[i].size() >= m_columnWidth.size())
                            {
                                rowFound = true;
                                break;
                            }
                        }

                        // Erase the last column if no other row is using it
                        if (!rowFound)
                            m_columnWidth.erase(m_columnWidth.end()-1);
                    }

                    // If the row is empty then remove it as well
                    if (m_gridWidgets[row].empty())
                    {
                        m_gridWidgets.erase(m_gridWidgets.begin() + row);
                        m_objPadding.erase(m_objPadding.begin() + row);
                        m_objAlignment.erase(m_objAlignment.begin() + row);
                        m_rowHeight.erase(m_rowHeight.begin() + row);
                    }

                    // Update the positions of all remaining widgets
                    updatePositionsOfAllWidgets();
                }
            }
        }

        return Container::remove(widget);
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void Grid::removeAllWidgets()
    {
        Container::removeAllWidgets();

        m_gridWidgets.clear();
        m_objPadding.clear();
        m_objAlignment.clear();

        m_rowHeight.clear();
        m_columnWidth.clear();

        m_connectedCallbacks.clear();

        updateWidgets();
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void Grid::addWidget(const Widget::Ptr& widget, std::size_t row, std::size_t col, const Padding& padding, Alignment alignment)
    {
        // If the widget hasn't already been added then add it now
        if (std::find(getWidgets().begin(), getWidgets().end(), widget) == getWidgets().end())
            add(widget);

        // Create the row if it did not exist yet
        if (m_gridWidgets.size() < row + 1)
        {
            m_gridWidgets.resize(row + 1);
            m_objPadding.resize(row + 1);
            m_objAlignment.resize(row + 1);
        }

        // Create the column if it did not exist yet
        if (m_gridWidgets[row].size() < col + 1)
        {
            m_gridWidgets[row].resize(col + 1, nullptr);
            m_objPadding[row].resize(col + 1);
            m_objAlignment[row].resize(col + 1);
        }

        // If this is a new row then reserve some space for it
        if (m_rowHeight.size() < row + 1)
            m_rowHeight.resize(row + 1, 0);

        // If this is the first row to have so many columns then reserve some space for it
        if (m_columnWidth.size() < col + 1)
            m_columnWidth.resize(col + 1, 0);

        // Add the widget to the grid
        m_gridWidgets[row][col] = widget;
        m_objPadding[row][col] = padding;
        m_objAlignment[row][col] = alignment;

        // Update the widgets
        updateWidgets();

        // Automatically update the widgets when their size changes
        m_connectedCallbacks[widget] = widget->connect("SizeChanged", [this](){ updateWidgets(); });
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    Widget::Ptr Grid::getWidget(std::size_t row, std::size_t col) const
    {
        if ((row < m_gridWidgets.size()) && (col < m_gridWidgets[row].size()))
            return m_gridWidgets[row][col];
        else
            return nullptr;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    std::map<Widget::Ptr, std::pair<std::size_t, std::size_t>> Grid::getWidgetLocations() const
    {
        std::map<Widget::Ptr, std::pair<std::size_t, std::size_t>> widgetsMap;

        for (std::size_t row = 0; row < m_gridWidgets.size(); ++row)
        {
            for (std::size_t col = 0; col < m_gridWidgets[row].size(); ++col)
            {
                if (m_gridWidgets[row][col])
                    widgetsMap[m_gridWidgets[row][col]] = {row, col};
            }
        }

        return widgetsMap;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void Grid::setWidgetPadding(const Widget::Ptr& widget, const Padding& padding)
    {
        // Find the widget in the grid
        for (std::size_t row = 0; row < m_gridWidgets.size(); ++row)
        {
            for (std::size_t col = 0; col < m_gridWidgets[row].size(); ++col)
            {
                if (m_gridWidgets[row][col] == widget)
                    setWidgetPadding(row, col, padding);
            }
        }
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void Grid::setWidgetPadding(std::size_t row, std::size_t col, const Padding& padding)
    {
        if (((row < m_gridWidgets.size()) && (col < m_gridWidgets[row].size())) && (m_gridWidgets[row][col] != nullptr))
        {
            // Change padding of the widget
            m_objPadding[row][col] = padding;

            // Update all widgets
            updateWidgets();
        }
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    Padding Grid::getWidgetPadding(const Widget::Ptr& widget) const
    {
        // Find the widget in the grid
        for (std::size_t row = 0; row < m_gridWidgets.size(); ++row)
        {
            for (std::size_t col = 0; col < m_gridWidgets[row].size(); ++col)
            {
                if (m_gridWidgets[row][col] == widget)
                    return getWidgetPadding(row, col);
            }
        }

        return {};
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    Padding Grid::getWidgetPadding(std::size_t row, std::size_t col) const
    {
        if (((row < m_gridWidgets.size()) && (col < m_gridWidgets[row].size())) && (m_gridWidgets[row][col] != nullptr))
            return m_objPadding[row][col];
        else
            return {};
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void Grid::setWidgetAlignment(const Widget::Ptr& widget, Alignment alignment)
    {
        // Find the widget in the grid
        for (std::size_t row = 0; row < m_gridWidgets.size(); ++row)
        {
            for (std::size_t col = 0; col < m_gridWidgets[row].size(); ++col)
            {
                if (m_gridWidgets[row][col] == widget)
                    setWidgetAlignment(row, col, alignment);
            }
        }
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void Grid::setWidgetAlignment(std::size_t row, std::size_t col, Alignment alignment)
    {
        if (((row < m_gridWidgets.size()) && (col < m_gridWidgets[row].size())) && (m_gridWidgets[row][col] != nullptr))
        {
            m_objAlignment[row][col] = alignment;
            updatePositionsOfAllWidgets();
        }
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    Grid::Alignment Grid::getWidgetAlignment(const Widget::Ptr& widget) const
    {
        for (std::size_t row = 0; row < m_gridWidgets.size(); ++row)
        {
            for (std::size_t col = 0; col < m_gridWidgets[row].size(); ++col)
            {
                if (m_gridWidgets[row][col] == widget)
                    return getWidgetAlignment(row, col);
            }
        }

        return Alignment::Center;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    Grid::Alignment Grid::getWidgetAlignment(std::size_t row, std::size_t col) const
    {
        if (((row < m_gridWidgets.size()) && (col < m_gridWidgets[row].size())) && (m_gridWidgets[row][col] != nullptr))
            return m_objAlignment[row][col];
        else
            return Alignment::Center;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    const std::vector<std::vector<Widget::Ptr>>& Grid::getGridWidgets() const
    {
        return m_gridWidgets;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    bool Grid::mouseOnWidget(Vector2f pos) const
    {
        return FloatRect{getPosition().x, getPosition().y, getSize().x, getSize().y}.contains(pos);
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    std::unique_ptr<DataIO::Node> Grid::save(SavingRenderersMap& renderers) const
    {
        auto node = Container::save(renderers);

        const auto& children = getWidgets();
        auto widgetsMap = getWidgetLocations();
        if (!widgetsMap.empty())
        {
            auto alignmentToString = [](Grid::Alignment alignment) -> std::string {
                switch (alignment)
                {
                case Grid::Alignment::Center:
                    return "Center";
                case Grid::Alignment::UpperLeft:
                    return "UpperLeft";
                case Grid::Alignment::Up:
                    return "Up";
                case Grid::Alignment::UpperRight:
                    return "UpperRight";
                case Grid::Alignment::Right:
                    return "Right";
                case Grid::Alignment::BottomRight:
                    return "BottomRight";
                case Grid::Alignment::Bottom:
                    return "Bottom";
                case Grid::Alignment::BottomLeft:
                    return "BottomLeft";
                case Grid::Alignment::Left:
                    return "Left";
                default:
                    throw Exception{"Invalid grid alignment encountered."};
                }
            };

            auto getWidgetsInGridString = [&](const Widget::Ptr& w) -> std::string {
                auto it = widgetsMap.find(w);
                if (it != widgetsMap.end())
                {
                    const auto row = it->second.first;
                    const auto col = it->second.second;
                    return "\"(" + to_string(row)
                         + ", " + to_string(col)
                         + ", " + getWidgetPadding(row, col).toString()
                         + ", " + alignmentToString(getWidgetAlignment(row, col))
                         + ")\"";
                }
                else
                    return "\"()\"";
            };

            std::string str = "[" + getWidgetsInGridString(children[0]);

            for (std::size_t i = 1; i < children.size(); ++i)
                str += ", " + getWidgetsInGridString(children[i]);

            str += "]";
            node->propertyValuePairs["GridWidgets"] = std::make_unique<DataIO::ValueNode>(str);
        }

        if (m_autoSize)
            node->propertyValuePairs.erase("Size");

        node->propertyValuePairs["AutoSize"] = std::make_unique<DataIO::ValueNode>(to_string(m_autoSize));
        return node;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void Grid::load(const std::unique_ptr<DataIO::Node>& node, const LoadingRenderersMap& renderers)
    {
        Container::load(node, renderers);

        if (node->propertyValuePairs["autosize"])
            setAutoSize(Deserializer::deserialize(ObjectConverter::Type::Bool, node->propertyValuePairs["autosize"]->value).getBool());

        if (node->propertyValuePairs["gridwidgets"])
        {
            if (!node->propertyValuePairs["gridwidgets"]->listNode)
                throw Exception{"Failed to parse 'GridWidgets' property, expected a list as value"};

            const auto& elements = node->propertyValuePairs["gridwidgets"]->valueList;
            if (elements.size() != getWidgets().size())
                throw Exception{"Failed to parse 'GridWidgets' property, the amount of items has to match with the amount of child widgets"};

            for (unsigned int i = 0; i < elements.size(); ++i)
            {
                std::string str = elements[i].toAnsiString();

                // Remove quotes
                if ((str.size() >= 2) && (str[0] == '"') && (str[str.size()-1] == '"'))
                    str = str.substr(1, str.size()-2);

                // Remove brackets
                if ((str.size() >= 2) && (str[0] == '(') && (str[str.size()-1] == ')'))
                    str = str.substr(1, str.size()-2);

                // Ignore empty values (which are widgets that have not been given a location in the grid)
                if (str.empty())
                    continue;

                int row;
                int col;
                Padding padding;
                auto alignment = Grid::Alignment::Center;

                std::size_t index = 0;
                std::size_t pos = str.find(',');
                if (pos == std::string::npos)
                    throw Exception{"Failed to parse 'GridWidgets' property. Expected list values to be in the form of '\"(row, column, (padding), alignment)\"'. Missing comma after row."};

                row = tgui::stoi(str.substr(index, pos - index));
                index = pos + 1;

                pos = str.find(',', index);
                if (pos == std::string::npos)
                    throw Exception{"Failed to parse 'GridWidgets' property. Expected list values to be in the form of '\"(row, column, (padding), alignment)\"'. Missing comma after column."};

                col = tgui::stoi(str.substr(index, pos - index));
                index = pos + 1;

                if (row < 0 || col < 0)
                    throw Exception{"Failed to parse 'GridWidgets' property, row and column have to be positive integers"};

                pos = str.find('(', index);
                if (pos == std::string::npos)
                    throw Exception{"Failed to parse 'GridWidgets' property. Expected list values to be in the form of '\"(row, column, (padding), alignment)\"'. Missing opening bracket for padding."};

                index = pos;
                pos = str.find(')', index);
                if (pos == std::string::npos)
                    throw Exception{"Failed to parse 'GridWidgets' property. Expected list values to be in the form of '\"(row, column, (padding), alignment)\"'. Missing closing bracket for padding."};

                padding = Deserializer::deserialize(ObjectConverter::Type::Outline, str.substr(index, pos+1 - index)).getOutline();
                index = pos + 1;

                pos = str.find(',', index);
                if (pos == std::string::npos)
                    throw Exception{"Failed to parse 'GridWidgets' property. Expected list values to be in the form of '\"(row, column, (padding), alignment)\"'. Missing comma after padding."};

                std::string alignmentStr = toLower(trim(str.substr(pos + 1)));
                if (alignmentStr == "center")
                    alignment = Grid::Alignment::Center;
                else if (alignmentStr == "upperleft")
                    alignment = Grid::Alignment::UpperLeft;
                else if (alignmentStr == "up")
                    alignment = Grid::Alignment::Up;
                else if (alignmentStr == "upperright")
                    alignment = Grid::Alignment::UpperRight;
                else if (alignmentStr == "right")
                    alignment = Grid::Alignment::Right;
                else if (alignmentStr == "bottomright")
                    alignment = Grid::Alignment::BottomRight;
                else if (alignmentStr == "bottom")
                    alignment = Grid::Alignment::Bottom;
                else if (alignmentStr == "bottomleft")
                    alignment = Grid::Alignment::BottomLeft;
                else if (alignmentStr == "left")
                    alignment = Grid::Alignment::Left;
                else
                    throw Exception{"Failed to parse 'GridWidgets' property. Invalid alignment '" + alignmentStr + "'."};

                addWidget(getWidgets()[i], static_cast<std::size_t>(row), static_cast<std::size_t>(col), padding, alignment);
            }
        }
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    Vector2f Grid::getMinimumSize() const
    {
        // Calculate the required space to have all widgets in the grid.
        Vector2f minSize;

        // Loop through all rows to find the minimum height required by the grid
        for (float rowHeight : m_rowHeight)
            minSize.y += rowHeight;

        // Loop through all columns to find the minimum width required by the grid
        for (float columnWidth : m_columnWidth)
            minSize.x += columnWidth;

        return minSize;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void Grid::updatePositionsOfAllWidgets()
    {
        Vector2f extraOffset;
        if (!m_autoSize && m_gridWidgets.size() > 1)
        {
            const Vector2f minimumSize = getMinimumSize();
            if (getSize().x > minimumSize.x)
                extraOffset.x = (getSize().x - minimumSize.x) / (m_gridWidgets.size() - 1);
            if (getSize().y > minimumSize.y)
                extraOffset.y = (getSize().y - minimumSize.y) / (m_gridWidgets.size() - 1);
        }

        Vector2f position;
        for (std::size_t row = 0; row < m_gridWidgets.size(); ++row)
        {
            const Vector2f previousPosition = position;

            for (std::size_t col = 0; col < m_gridWidgets[row].size(); ++col)
            {
                if (m_gridWidgets[row][col].get() == nullptr)
                {
                    position.x += m_columnWidth[col] + extraOffset.x;
                    continue;
                }

                Vector2f cellPosition = position + (extraOffset / 2.f);
                switch (m_objAlignment[row][col])
                {
                case Alignment::UpperLeft:
                    cellPosition.x += m_objPadding[row][col].getLeft();
                    cellPosition.y += m_objPadding[row][col].getTop();
                    break;

                case Alignment::Up:
                    cellPosition.x += m_objPadding[row][col].getLeft() + (((m_columnWidth[col] - m_objPadding[row][col].getLeft() - m_objPadding[row][col].getRight()) - m_gridWidgets[row][col]->getFullSize().x) / 2.f);
                    cellPosition.y += m_objPadding[row][col].getTop();
                    break;

                case Alignment::UpperRight:
                    cellPosition.x += m_columnWidth[col] - m_objPadding[row][col].getRight() - m_gridWidgets[row][col]->getFullSize().x;
                    cellPosition.y += m_objPadding[row][col].getTop();
                    break;

                case Alignment::Right:
                    cellPosition.x += m_columnWidth[col] - m_objPadding[row][col].getRight() - m_gridWidgets[row][col]->getFullSize().x;
                    cellPosition.y += m_objPadding[row][col].getTop() + (((m_rowHeight[row] - m_objPadding[row][col].getTop() - m_objPadding[row][col].getBottom()) - m_gridWidgets[row][col]->getFullSize().y) / 2.f);
                    break;

                case Alignment::BottomRight:
                    cellPosition.x += m_columnWidth[col] - m_objPadding[row][col].getRight() - m_gridWidgets[row][col]->getFullSize().x;
                    cellPosition.y += m_rowHeight[row] - m_objPadding[row][col].getBottom() - m_gridWidgets[row][col]->getFullSize().y;
                    break;

                case Alignment::Bottom:
                    cellPosition.x += m_objPadding[row][col].getLeft() + (((m_columnWidth[col] - m_objPadding[row][col].getLeft() - m_objPadding[row][col].getRight()) - m_gridWidgets[row][col]->getFullSize().x) / 2.f);
                    cellPosition.y += m_rowHeight[row] - m_objPadding[row][col].getBottom() - m_gridWidgets[row][col]->getFullSize().y;
                    break;

                case Alignment::BottomLeft:
                    cellPosition.x += m_objPadding[row][col].getLeft();
                    cellPosition.y += m_rowHeight[row] - m_objPadding[row][col].getBottom() - m_gridWidgets[row][col]->getFullSize().y;
                    break;

                case Alignment::Left:
                    cellPosition.x += m_objPadding[row][col].getLeft();
                    cellPosition.y += m_objPadding[row][col].getTop() + (((m_rowHeight[row] - m_objPadding[row][col].getTop() - m_objPadding[row][col].getBottom()) - m_gridWidgets[row][col]->getFullSize().y) / 2.f);
                    break;

                case Alignment::Center:
                    cellPosition.x += m_objPadding[row][col].getLeft() + (((m_columnWidth[col] - m_objPadding[row][col].getLeft() - m_objPadding[row][col].getRight()) - m_gridWidgets[row][col]->getFullSize().x) / 2.f);
                    cellPosition.y += m_objPadding[row][col].getTop() + (((m_rowHeight[row] - m_objPadding[row][col].getTop() - m_objPadding[row][col].getBottom()) - m_gridWidgets[row][col]->getFullSize().y) / 2.f);
                    break;
                }

                m_gridWidgets[row][col]->setPosition(cellPosition);
                position.x += m_columnWidth[col] + extraOffset.x;
            }

            // Move to the next row
            position = previousPosition;
            position.y += m_rowHeight[row] + extraOffset.y;
        }
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void Grid::updateWidgets()
    {
        // Reset the column widths
        for (float& width : m_columnWidth)
            width = 0;

        // Loop through all widgets
        for (std::size_t row = 0; row < m_gridWidgets.size(); ++row)
        {
            // Reset the row height
            m_rowHeight[row] = 0;

            for (std::size_t col = 0; col < m_gridWidgets[row].size(); ++col)
            {
                if (m_gridWidgets[row][col].get() == nullptr)
                    continue;

                // Remember the biggest column width
                if (m_columnWidth[col] < m_gridWidgets[row][col]->getFullSize().x + m_objPadding[row][col].getLeft() + m_objPadding[row][col].getRight())
                    m_columnWidth[col] = m_gridWidgets[row][col]->getFullSize().x + m_objPadding[row][col].getLeft() + m_objPadding[row][col].getRight();

                // Remember the biggest row height
                if (m_rowHeight[row] < m_gridWidgets[row][col]->getFullSize().y + m_objPadding[row][col].getTop() + m_objPadding[row][col].getBottom())
                    m_rowHeight[row] = m_gridWidgets[row][col]->getFullSize().y + m_objPadding[row][col].getTop() + m_objPadding[row][col].getBottom();
            }
        }

        if (m_autoSize)
        {
            sf::Vector2f size;
            for (std::size_t row = 0; row < m_gridWidgets.size(); ++row)
            {
                float rowWidth = 0;
                for (std::size_t col = 0; col < m_gridWidgets[row].size(); ++col)
                    rowWidth += m_columnWidth[col];

                size.x = std::max(size.x, rowWidth);
                size.y += m_rowHeight[row];
            }

            Container::setSize(size);
        }

        updatePositionsOfAllWidgets();
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void Grid::draw(sf::RenderTarget& target, sf::RenderStates states) const
    {
        states.transform.translate(getPosition());

        // Draw all widgets
        for (std::size_t row = 0; row < m_gridWidgets.size(); ++row)
        {
            for (std::size_t col = 0; col < m_gridWidgets[row].size(); ++col)
            {
                if (m_gridWidgets[row][col].get() != nullptr)
                {
                    if (m_gridWidgets[row][col]->isVisible())
                        m_gridWidgets[row][col]->draw(target, states);
                }
            }
        }
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// TGUI - Texus' Graphical User Interface
// Copyright (C) 2012-2018 Bruno Van de Velde (vdv_b@tgui.eu)
//
// This software is provided 'as-is', without any express or implied warranty.
// In no event will the authors be held liable for any damages arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it freely,
// subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented;
//    you must not claim that you wrote the original software.
//    If you use this software in a product, an acknowledgment
//    in the product documentation would be appreciated but is not required.
//
// 2. Altered source versions must be plainly marked as such,
//    and must not be misrepresented as being the original software.
//
// 3. This notice may not be removed or altered from any source distribution.
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


#include <TGUI/Widgets/Knob.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <cmath>

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace
{
    const float pi = 3.14159265358979f;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace tgui
{
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    Knob::Knob()
    {
        m_type = "Knob";
        m_draggableWidget = true;

        m_renderer = aurora::makeCopied<KnobRenderer>();
        setRenderer(Theme::getDefault()->getRendererNoThrow(m_type));

        setSize(140, 140);
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    Knob::Ptr Knob::create()
    {
        return std::make_shared<Knob>();
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    Knob::Ptr Knob::copy(Knob::ConstPtr knob)
    {
        if (knob)
            return std::static_pointer_cast<Knob>(knob->clone());
        else
            return nullptr;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    KnobRenderer* Knob::getSharedRenderer()
    {
        return aurora::downcast<KnobRenderer*>(Widget::getSharedRenderer());
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    const KnobRenderer* Knob::getSharedRenderer() const
    {
        return aurora::downcast<const KnobRenderer*>(Widget::getSharedRenderer());
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    KnobRenderer* Knob::getRenderer()
    {
        return aurora::downcast<KnobRenderer*>(Widget::getRenderer());
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    const KnobRenderer* Knob::getRenderer() const
    {
        return aurora::downcast<const KnobRenderer*>(Widget::getRenderer());
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void Knob::setSize(const Layout2d& size)
    {
        Widget::setSize(size);

        m_bordersCached.updateParentSize(getSize());

        if (m_spriteBackground.isSet() && m_spriteForeground.isSet())
        {
            m_spriteBackground.setSize(getInnerSize());
            m_spriteForeground.setSize({m_spriteForeground.getTexture().getImageSize().x / m_spriteBackground.getTexture().getImageSize().x * getInnerSize().x,
                                        m_spriteForeground.getTexture().getImageSize().y / m_spriteBackground.getTexture().getImageSize().y * getInnerSize().y});
        }
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void Knob::setStartRotation(float startRotation)
    {
        while (startRotation >= 360)
            startRotation -= 360;
        while (startRotation < 0)
            startRotation += 360;

        m_startRotation = startRotation;

        // The knob might have to point in a different direction even though it has the same value
        recalculateRotation();
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    float Knob::getStartRotation() const
    {
        return m_startRotation;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void Knob::setEndRotation(float endRotation)
    {
        while (endRotation >= 360)
            endRotation -= 360;
        while (endRotation < 0)
            endRotation += 360;

        m_endRotation = endRotation;

        // The knob might have to point in a different direction even though it has the same value
        recalculateRotation();
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    float Knob::getEndRotation() const
    {
        return m_endRotation;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void Knob::setMinimum(int minimum)
    {
        if (m_minimum != minimum)
        {
            // Set the new minimum
            m_minimum = minimum;

            // The maximum can't be below the minimum
            if (m_maximum < m_minimum)
                m_maximum = m_minimum;

            // When the value is below the minimum then adjust it
            if (m_value < m_minimum)
                setValue(m_minimum);

            // The knob might have to point in a different direction even though it has the same value
            recalculateRotation();
        }
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    int Knob::getMinimum() const
    {
        return m_minimum;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void Knob::setMaximum(int maximum)
    {
        if (m_maximum != maximum)
        {
            // Set the new maximum
            if (maximum > 0)
                m_maximum = maximum;
            else
                m_maximum = 1;

            // The minimum can't be below the maximum
            if (m_minimum > m_maximum)
                m_minimum = m_maximum;

            // When the value is above the maximum then adjust it
            if (m_value > m_maximum)
                setValue(m_maximum);

            // The knob might have to point in a different direction even though it has the same value
            recalculateRotation();
        }
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    int Knob::getMaximum() const
    {
        return m_maximum;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void Knob::setValue(int value)
    {
        if (m_value != value)
        {
            // Set the new value
            m_value = value;

            // When the value is below the minimum or above the maximum then adjust it
            if (m_value < m_minimum)
                m_value = m_minimum;
            else if (m_value > m_maximum)
                m_value = m_maximum;

            // The knob might have to point in a different direction
            recalculateRotation();

            onValueChange.emit(this, m_value);
        }
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    int Knob::getValue() const
    {
        return m_value;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void Knob::setClockwiseTurning(bool clockwise)
    {
        m_clockwiseTurning = clockwise;

        // The knob might have to point in a different direction even though it has the same value
        recalculateRotation();
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    bool Knob::getClockwiseTurning() const
    {
        return m_clockwiseTurning;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    bool Knob::mouseOnWidget(Vector2f pos) const
    {
        pos -= getPosition();

        // Check if the mouse is on top of the widget
        if (FloatRect{0, 0, getSize().x, getSize().y}.contains(pos))
        {
            if (m_spriteBackground.isSet() && m_spriteForeground.isSet())
            {
                // Only return true when the pixel under the mouse isn't transparent
                if (!m_spriteBackground.isTransparentPixel(pos - m_bordersCached.getOffset()))
                    return true;
            }
            else // There is no texture, the widget has a circle shape
            {
                const Vector2f centerPoint = getSize() / 2.f;
                const float distance = std::sqrt(std::pow(centerPoint.x - pos.x, 2.f) + std::pow(centerPoint.y - pos.y, 2.f));
                return (distance <= std::min(getSize().x, getSize().y));
            }
        }

        return false;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void Knob::leftMousePressed(Vector2f pos)
    {
        // Set the mouse down flag
        m_mouseDown = true;

        // Change the value of the knob depending on where you clicked
        mouseMoved(pos);
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void Knob::leftMouseReleased(Vector2f)
    {
        m_mouseDown = false;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void Knob::mouseMoved(Vector2f pos)
    {
        pos -= getPosition();

        if (!m_mouseHover)
            mouseEnteredWidget();

        const Vector2f centerPosition = getSize() / 2.0f;

        // Check if the mouse button is down
        if (m_mouseDown)
        {
            // Find out the direction that the knob should now point
            if (compareFloats(pos.x, centerPosition.x))
            {
                if (pos.y > centerPosition.y)
                    m_angle = 270;
                else if (pos.y < centerPosition.y)
                    m_angle = 90;
            }
            else
            {
                m_angle = std::atan2(centerPosition.y - pos.y, pos.x - centerPosition.x) * 180.0f / pi;
                if (m_angle < 0)
                    m_angle += 360;
            }

            // The angle might lie on a part where it isn't allowed
            if (m_angle > m_startRotation)
            {
                if ((m_angle < m_endRotation) && (m_clockwiseTurning))
                {
                    if ((m_angle - m_startRotation) <= (m_endRotation - m_angle))
                        m_angle = m_startRotation;
                    else
                        m_angle = m_endRotation;
                }
                else if (m_angle > m_endRotation)
                {
                    if (((m_startRotation > m_endRotation) && (m_clockwiseTurning))
                     || ((m_startRotation < m_endRotation) && (!m_clockwiseTurning)))
                    {
                        if (std::min(m_angle - m_startRotation, 360 - m_angle + m_startRotation) <= std::min(m_angle - m_endRotation, 360 - m_angle + m_endRotation))
                            m_angle = m_startRotation;
                        else
                            m_angle = m_endRotation;
                    }
                }
            }
            else if (m_angle < m_startRotation)
            {
                if (m_angle < m_endRotation)
                {
                    if (((m_startRotation > m_endRotation) && (m_clockwiseTurning))
                     || ((m_startRotation < m_endRotation) && (!m_clockwiseTurning)))
                    {
                        if (std::min(m_startRotation - m_angle, 360 - m_startRotation + m_angle) <= std::min(m_endRotation - m_angle, 360 -m_endRotation + m_angle))
                            m_angle = m_startRotation;
                        else
                            m_angle = m_endRotation;
                    }
                }
                else if ((m_angle > m_endRotation) && (!m_clockwiseTurning))
                {
                    if ((m_startRotation - m_angle) <= (m_angle - m_endRotation))
                        m_angle = m_startRotation;
                    else
                        m_angle = m_endRotation;
                }
            }

            // Calculate the difference in degrees between the start and end rotation
            float allowedAngle = 0;
            if (compareFloats(m_startRotation, m_endRotation))
                allowedAngle = 360;
            else
            {
                if (((m_endRotation > m_startRotation) && (m_clockwiseTurning))
                 || ((m_endRotation < m_startRotation) && (!m_clockwiseTurning)))
                {
                    allowedAngle = 360 - std::abs(m_endRotation - m_startRotation);
                }
                else if (((m_endRotation > m_startRotation) && (!m_clockwiseTurning))
                      || ((m_endRotation < m_startRotation) && (m_clockwiseTurning)))
                {
                    allowedAngle = std::abs(m_endRotation - m_startRotation);
                }
            }

            // Calculate the right value
            if (m_clockwiseTurning)
            {
                if (m_angle < m_startRotation)
                    setValue(static_cast<int>(((m_startRotation - m_angle) / allowedAngle * (m_maximum - m_minimum)) + m_minimum));
                else
                {
                    if (compareFloats(m_angle, m_startRotation))
                        setValue(m_minimum);
                    else
                        setValue(static_cast<int>((((360.0f - m_angle) + m_startRotation) / allowedAngle * (m_maximum - m_minimum)) + m_minimum));
                }
            }
            else // counter-clockwise
            {
                if (m_angle >= m_startRotation)
                    setValue(static_cast<int>(((m_angle - m_startRotation) / allowedAngle * (m_maximum - m_minimum)) + m_minimum));
                else
                {
                    setValue(static_cast<int>(((m_angle + (360.0f - m_startRotation)) / allowedAngle * (m_maximum - m_minimum)) + m_minimum));
                }
            }
        }
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void Knob::recalculateRotation()
    {
        // Calculate the difference in degrees between the start and end rotation
        float allowedAngle = 0;
        if (compareFloats(m_startRotation, m_endRotation))
            allowedAngle = 360;
        else
        {
            if (((m_endRotation > m_startRotation) && (m_clockwiseTurning))
             || ((m_endRotation < m_startRotation) && (!m_clockwiseTurning)))
            {
                allowedAngle = 360 - std::abs(m_endRotation - m_startRotation);
            }
            else if (((m_endRotation > m_startRotation) && (!m_clockwiseTurning))
                  || ((m_endRotation < m_startRotation) && (m_clockwiseTurning)))
            {
                allowedAngle = std::abs(m_endRotation - m_startRotation);
            }
        }

        // Calculate the angle for the direction of the knob
        if (m_clockwiseTurning)
        {
            if (m_value == m_minimum)
                m_angle = m_startRotation;
            else
                m_angle = m_startRotation - (((m_value - m_minimum) / static_cast<float>(m_maximum - m_minimum)) * allowedAngle);
        }
        else // counter-clockwise
        {
            m_angle = (((m_value - m_minimum) / static_cast<float>(m_maximum - m_minimum)) * allowedAngle) + m_startRotation;
        }
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    Signal& Knob::getSignal(std::string signalName)
    {
        if (signalName == toLower(onValueChange.getName()))
            return onValueChange;
        else
            return Widget::getSignal(std::move(signalName));
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void Knob::rendererChanged(const std::string& property)
    {
        if (property == "borders")
        {
            m_bordersCached = getSharedRenderer()->getBorders();
            setSize(m_size);
        }
        else if (property == "texturebackground")
        {
            m_spriteBackground.setTexture(getSharedRenderer()->getTextureBackground());
            setSize(m_size);
        }
        else if (property == "textureforeground")
        {
            m_spriteForeground.setTexture(getSharedRenderer()->getTextureForeground());
            setSize(m_size);
        }
        else if (property == "bordercolor")
        {
            m_borderColorCached = getSharedRenderer()->getBorderColor();
        }
        else if (property == "backgroundcolor")
        {
            m_backgroundColorCached = getSharedRenderer()->getBackgroundColor();
        }
        else if (property == "thumbcolor")
        {
            m_thumbColorCached = getSharedRenderer()->getThumbColor();
        }
        else if (property == "imagerotation")
        {
            m_imageRotationCached = getSharedRenderer()->getImageRotation();
        }
        else if (property == "opacity")
        {
            Widget::rendererChanged(property);

            m_spriteBackground.setOpacity(m_opacityCached);
            m_spriteForeground.setOpacity(m_opacityCached);
        }
        else
            Widget::rendererChanged(property);
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    std::unique_ptr<DataIO::Node> Knob::save(SavingRenderersMap& renderers) const
    {
        auto node = Widget::save(renderers);

        node->propertyValuePairs["ClockwiseTurning"] = std::make_unique<DataIO::ValueNode>(Serializer::serialize(m_clockwiseTurning));
        node->propertyValuePairs["StartRotation"] = std::make_unique<DataIO::ValueNode>(to_string(m_startRotation));
        node->propertyValuePairs["EndRotation"] = std::make_unique<DataIO::ValueNode>(to_string(m_endRotation));
        node->propertyValuePairs["Minimum"] = std::make_unique<DataIO::ValueNode>(to_string(m_minimum));
        node->propertyValuePairs["Maximum"] = std::make_unique<DataIO::ValueNode>(to_string(m_maximum));
        node->propertyValuePairs["Value"] = std::make_unique<DataIO::ValueNode>(to_string(m_value));

        return node;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void Knob::load(const std::unique_ptr<DataIO::Node>& node, const LoadingRenderersMap& renderers)
    {
        Widget::load(node, renderers);

        if (node->propertyValuePairs["startrotation"])
            setStartRotation(tgui::stof(node->propertyValuePairs["startrotation"]->value));
        if (node->propertyValuePairs["endrotation"])
            setEndRotation(tgui::stof(node->propertyValuePairs["endrotation"]->value));
        if (node->propertyValuePairs["minimum"])
            setMinimum(tgui::stoi(node->propertyValuePairs["minimum"]->value));
        if (node->propertyValuePairs["maximum"])
            setMaximum(tgui::stoi(node->propertyValuePairs["maximum"]->value));
        if (node->propertyValuePairs["value"])
            setValue(tgui::stoi(node->propertyValuePairs["value"]->value));
        if (node->propertyValuePairs["clockwiseturning"])
            setClockwiseTurning(Deserializer::deserialize(ObjectConverter::Type::Bool, node->propertyValuePairs["clockwiseturning"]->value).getBool());
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    Vector2f Knob::getInnerSize() const
    {
        return {getSize().x - m_bordersCached.getLeft() - m_bordersCached.getRight(), getSize().y - m_bordersCached.getTop() - m_bordersCached.getBottom()};
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void Knob::draw(sf::RenderTarget& target, sf::RenderStates states) const
    {
        states.transform.translate(getPosition());

        const float size = std::min(getInnerSize().x, getInnerSize().y);

        // Draw the borders
        const float borderThickness = std::min({m_bordersCached.getLeft(), m_bordersCached.getTop(), m_bordersCached.getRight(), m_bordersCached.getBottom()});
        if (borderThickness > 0)
        {
            states.transform.translate({borderThickness, borderThickness});

            sf::CircleShape bordersShape{size / 2};
            bordersShape.setFillColor(Color::Transparent);
            bordersShape.setOutlineColor(Color::calcColorOpacity(m_borderColorCached, m_opacityCached));
            bordersShape.setOutlineThickness(borderThickness);
            target.draw(bordersShape, states);
        }

        // Draw the background
        if (m_spriteBackground.isSet())
            m_spriteBackground.draw(target, states);
        else
        {
            sf::CircleShape background{size / 2};
            background.setFillColor(Color::calcColorOpacity(m_backgroundColorCached, m_opacityCached));
            target.draw(background, states);
        }

        // Draw the foreground
        if (m_spriteForeground.isSet())
        {
            states.transform.translate((getInnerSize() - m_spriteForeground.getSize()) / 2.f);

            // Give the image the correct rotation
            if (m_imageRotationCached > m_angle)
                states.transform.rotate(m_imageRotationCached - m_angle, (m_spriteForeground.getSize() / 2.f));
            else
                states.transform.rotate(360 - m_angle + m_imageRotationCached, (m_spriteForeground.getSize() / 2.f));

            m_spriteForeground.draw(target, states);
        }
        else
        {
            sf::CircleShape thumb{size / 10.0f};
            thumb.setFillColor(Color::calcColorOpacity(m_thumbColorCached, m_opacityCached));
            thumb.setPosition({(size / 2.0f) - thumb.getRadius() + (std::cos(m_angle / 180 * pi) * (size / 2) * 3/5),
                               (size / 2.0f) - thumb.getRadius() + (-std::sin(m_angle / 180 * pi) * (size / 2) * 3/5)});
            target.draw(thumb, states);
        }
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// TGUI - Texus' Graphical User Interface
// Copyright (C) 2012-2018 Bruno Van de Velde (vdv_b@tgui.eu)
//
// This software is provided 'as-is', without any express or implied warranty.
// In no event will the authors be held liable for any damages arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it freely,
// subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented;
//    you must not claim that you wrote the original software.
//    If you use this software in a product, an acknowledgment
//    in the product documentation would be appreciated but is not required.
//
// 2. Altered source versions must be plainly marked as such,
//    and must not be misrepresented as being the original software.
//
// 3. This notice may not be removed or altered from any source distribution.
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


#include <TGUI/Widgets/MenuBar.hpp>
#include <TGUI/Container.hpp>
#include <TGUI/Clipping.hpp>
#include <SFML/Graphics/ConvexShape.hpp>

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace tgui
{
    namespace
    {
        bool removeMenuImpl(const std::vector<sf::String>& hierarchy, bool removeParentsWhenEmpty, unsigned int parentIndex, std::vector<MenuBar::Menu>& menus)
        {
            for (auto it = menus.begin(); it != menus.end(); ++it)
            {
                if (it->text.getString() != hierarchy[parentIndex])
                    continue;

                if (parentIndex + 1 == hierarchy.size())
                {
                    menus.erase(it);
                    return true;
                }
                else
                {
                    // Return false if some menu in the hierarchy couldn't be found
                    if (!removeMenuImpl(hierarchy, removeParentsWhenEmpty, parentIndex + 1, it->menuItems))
                        return false;

                    // If parents don't have to be removed as well then we are done
                    if (!removeParentsWhenEmpty)
                        return true;

                    // Also delete the parent if empty
                    if (it->menuItems.empty())
                        menus.erase(it);

                    return true;
                }
            }

            // The hierarchy doesn't exist
            return false;
        }

        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

        void setTextSizeImpl(std::vector<MenuBar::Menu>& menus, unsigned int textSize)
        {
            for (auto& menu : menus)
            {
                menu.text.setCharacterSize(textSize);
                if (!menu.menuItems.empty())
                    setTextSizeImpl(menu.menuItems, textSize);
            }
        }

        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

        std::vector<std::unique_ptr<MenuBar::GetAllMenusElement>> getAllMenusImpl(const std::vector<MenuBar::Menu>& menus)
        {
            std::vector<std::unique_ptr<MenuBar::GetAllMenusElement>> menuElements;

            for (const auto& menu : menus)
            {
                menuElements.emplace_back(std::make_unique<MenuBar::GetAllMenusElement>());
                menuElements.back()->text = menu.text.getString();
                menuElements.back()->enabled = menu.enabled;
                if (!menu.menuItems.empty())
                    menuElements.back()->menuItems = getAllMenusImpl(menu.menuItems);
            }

            return menuElements;
        }

        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

        std::vector<MenuBar::GetMenuListElement> getMenuListImpl(const std::vector<MenuBar::Menu>& menus)
        {
            std::vector<MenuBar::GetMenuListElement> menuElements;

            for (const auto& menu : menus)
            {
                menuElements.emplace_back();
                menuElements.back().text = menu.text.getString();
                menuElements.back().enabled = menu.enabled;
                if (!menu.menuItems.empty())
                    menuElements.back().menuItems = getMenuListImpl(menu.menuItems);
            }

            return menuElements;
        }

        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

        void saveMenus(std::unique_ptr<DataIO::Node>& parentNode, const std::vector<MenuBar::Menu>& menus)
        {
            for (const auto& menu : menus)
            {
                auto menuNode = std::make_unique<DataIO::Node>();
                menuNode->name = "Menu";

                menuNode->propertyValuePairs["Text"] = std::make_unique<DataIO::ValueNode>(Serializer::serialize(menu.text.getString()));
                if (!menu.enabled)
                    menuNode->propertyValuePairs["Enabled"] = std::make_unique<DataIO::ValueNode>(Serializer::serialize(menu.enabled));

                if (!menu.menuItems.empty())
                {
                    // Save as nested 'Menu' sections only when needed, use the more compact string list when just storing the menu items
                    bool recursionNeeded = false;
                    for (const auto& menuItem : menu.menuItems)
                    {
                        if (!menuItem.enabled || !menuItem.menuItems.empty())
                        {
                            recursionNeeded = true;
                            break;
                        }
                    }

                    if (recursionNeeded)
                        saveMenus(menuNode, menu.menuItems);
                    else
                    {
                        std::string itemList = "[" + Serializer::serialize(menu.menuItems[0].text.getString());
                        for (std::size_t i = 1; i < menu.menuItems.size(); ++i)
                            itemList += ", " + Serializer::serialize(menu.menuItems[i].text.getString());
                        itemList += "]";

                        menuNode->propertyValuePairs["Items"] = std::make_unique<DataIO::ValueNode>(itemList);
                    }
                }

                parentNode->children.push_back(std::move(menuNode));
            }
        }
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    MenuBar::MenuBar()
    {
        m_type = "MenuBar";
        m_distanceToSideCached = Text::getLineHeight(m_fontCached, getGlobalTextSize()) * 0.4f;

        m_renderer = aurora::makeCopied<MenuBarRenderer>();
        setRenderer(Theme::getDefault()->getRendererNoThrow(m_type));

        setTextSize(getGlobalTextSize());
        setMinimumSubMenuWidth((Text::getLineHeight(m_fontCached, m_textSize) * 4) + (2 * m_distanceToSideCached));
        setSize({"100%", Text::getLineHeight(m_fontCached, m_textSize) * 1.25f});
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    MenuBar::Ptr MenuBar::create()
    {
        return std::make_shared<MenuBar>();
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    MenuBar::Ptr MenuBar::copy(MenuBar::ConstPtr menuBar)
    {
        if (menuBar)
            return std::static_pointer_cast<MenuBar>(menuBar->clone());
        else
            return nullptr;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    MenuBarRenderer* MenuBar::getSharedRenderer()
    {
        return aurora::downcast<MenuBarRenderer*>(Widget::getSharedRenderer());
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    const MenuBarRenderer* MenuBar::getSharedRenderer() const
    {
        return aurora::downcast<const MenuBarRenderer*>(Widget::getSharedRenderer());
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    MenuBarRenderer* MenuBar::getRenderer()
    {
        return aurora::downcast<MenuBarRenderer*>(Widget::getRenderer());
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    const MenuBarRenderer* MenuBar::getRenderer() const
    {
        return aurora::downcast<const MenuBarRenderer*>(Widget::getRenderer());
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void MenuBar::setSize(const Layout2d& size)
    {
        Widget::setSize(size);

        m_spriteBackground.setSize(getSize());
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void MenuBar::setEnabled(bool enabled)
    {
        Widget::setEnabled(enabled);

        if (!enabled)
        {
            closeMenu();
            updateTextColors(m_menus, m_visibleMenu);
        }
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void MenuBar::addMenu(const sf::String& text)
    {
        createMenu(m_menus, text);
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    bool MenuBar::addMenuItem(const sf::String& text)
    {
        if (!m_menus.empty())
            return addMenuItem(m_menus.back().text.getString(), text);
        else
            return false;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    bool MenuBar::addMenuItem(const sf::String& menu, const sf::String& text)
    {
        return addMenuItem({menu, text}, false);
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    bool MenuBar::addMenuItem(const std::vector<sf::String>& hierarchy, bool createParents)
    {
        if (hierarchy.size() < 2)
            return false;

        auto* menu = findMenu(hierarchy, 0, m_menus, createParents);
        if (!menu)
            return false;

        createMenu(menu->menuItems, hierarchy.back());
        return true;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void MenuBar::removeAllMenus()
    {
        m_menus.clear();
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    bool MenuBar::removeMenu(const sf::String& menu)
    {
        for (std::size_t i = 0; i < m_menus.size(); ++i)
        {
            if (m_menus[i].text.getString() != menu)
                continue;

            m_menus.erase(m_menus.begin() + i);

            // The menu was removed, so it can't remain open
            if (m_visibleMenu == static_cast<int>(i))
                m_visibleMenu = -1;

            return true;
        }

        // could not find the menu
        return false;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    bool MenuBar::removeMenuItem(const sf::String& menu, const sf::String& menuItem)
    {
        return removeMenuItem({menu, menuItem}, false);
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    bool MenuBar::removeMenuItem(const std::vector<sf::String>& hierarchy, bool removeParentsWhenEmpty)
    {
        if (hierarchy.size() < 2)
            return false;

        return removeMenuImpl(hierarchy, removeParentsWhenEmpty, 0, m_menus);
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    bool MenuBar::setMenuEnabled(const sf::String& menu, bool enabled)
    {
        for (std::size_t i = 0; i < m_menus.size(); ++i)
        {
            if (m_menus[i].text.getString() != menu)
                continue;

            if (!enabled && (m_visibleMenu == static_cast<int>(i)))
                closeMenu();

            m_menus[i].enabled = enabled;
            updateMenuTextColor(m_menus[i], (m_visibleMenu == static_cast<int>(i)));
            return true;
        }

        return false;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    bool MenuBar::getMenuEnabled(const sf::String& menuText) const
    {
        for (auto& menu : m_menus)
        {
            if (menu.text.getString() == menuText)
                return menu.enabled;
        }

        return false;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    bool MenuBar::setMenuItemEnabled(const sf::String& menuText, const sf::String& menuItemText, bool enabled)
    {
        return setMenuItemEnabled({menuText, menuItemText}, enabled);
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    bool MenuBar::setMenuItemEnabled(const std::vector<sf::String>& hierarchy, bool enabled)
    {
        if (hierarchy.size() < 2)
            return false;

        auto* menu = findMenu(hierarchy, 0, m_menus, false);
        if (!menu)
            return false;

        for (unsigned int j = 0; j < menu->menuItems.size(); ++j)
        {
            auto& menuItem = menu->menuItems[j];
            if (menuItem.text.getString() != hierarchy.back())
                continue;

            if (!enabled && (menu->selectedMenuItem == static_cast<int>(j)))
                menu->selectedMenuItem = -1;

            menuItem.enabled = enabled;
            updateMenuTextColor(menuItem, (menu->selectedMenuItem == static_cast<int>(j)));
            return true;
        }

        return false;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    bool MenuBar::getMenuItemEnabled(const sf::String& menuText, const sf::String& menuItemText) const
    {
        return getMenuItemEnabled({menuText, menuItemText});
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    bool MenuBar::getMenuItemEnabled(const std::vector<sf::String>& hierarchy) const
    {
        if (hierarchy.size() < 2)
            return false;

        auto* menuItem = findMenuItem(hierarchy);
        if (!menuItem)
            return false;

        return menuItem->enabled;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void MenuBar::setTextSize(unsigned int size)
    {
        m_textSize = size;
        setTextSizeImpl(m_menus, size);
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    unsigned int MenuBar::getTextSize() const
    {
        return m_textSize;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void MenuBar::setMinimumSubMenuWidth(float minimumWidth)
    {
        m_minimumSubMenuWidth = minimumWidth;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    float MenuBar::getMinimumSubMenuWidth() const
    {
        return m_minimumSubMenuWidth;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void MenuBar::setInvertedMenuDirection(bool invertDirection)
    {
        m_invertedMenuDirection = invertDirection;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    bool MenuBar::getInvertedMenuDirection() const
    {
        return m_invertedMenuDirection;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef TGUI_REMOVE_DEPRECATED_CODE
    std::vector<std::pair<sf::String, std::vector<sf::String>>> MenuBar::getMenus() const
    {
        std::vector<std::pair<sf::String, std::vector<sf::String>>> menus;

        for (const auto& menu : m_menus)
        {
            std::vector<sf::String> items;
            for (const auto& item : menu.menuItems)
                items.push_back(item.text.getString());

            menus.emplace_back(menu.text.getString(), std::move(items));
        }

        return menus;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    std::vector<std::unique_ptr<MenuBar::GetAllMenusElement>> MenuBar::getAllMenus() const
    {
        return getAllMenusImpl(m_menus);
    }
#endif

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    std::vector<MenuBar::GetMenuListElement> MenuBar::getMenuList() const
    {
        return getMenuListImpl(m_menus);
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void MenuBar::closeMenu()
    {
        if (m_visibleMenu != -1)
            closeSubMenus(m_menus, m_visibleMenu);
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    bool MenuBar::mouseOnWidget(Vector2f pos) const
    {
        if (FloatRect{getPosition().x, getPosition().y, getSize().x, getSize().y}.contains(pos))
            return true;
        else if (m_visibleMenu != -1)
        {
            Vector2f menuPos = getPosition();
            for (int i = 0; i < m_visibleMenu; ++i)
                menuPos.x += m_menus[i].text.getSize().x + (2 * m_distanceToSideCached);

            if (m_invertedMenuDirection)
                menuPos.y -= getSize().y * m_menus[m_visibleMenu].menuItems.size();
            else
                menuPos.y += getSize().y;

            // The menu is moved to the left if it otherwise falls off the screen
            bool openSubMenuToRight = true;
            const float menuWidth = calculateMenuWidth(m_menus[m_visibleMenu]);
            if (getParent() && (menuPos.x + menuWidth > getParent()->getInnerSize().x))
            {
                menuPos.x = std::max(0.f, getParent()->getInnerSize().x - menuWidth);
                openSubMenuToRight = false;
            }

            return isMouseOnTopOfMenu(menuPos, pos, openSubMenuToRight, m_menus[m_visibleMenu], menuWidth);
        }

        return false;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void MenuBar::leftMousePressed(Vector2f pos)
    {
        // Check if a menu should be opened or closed
        pos -= getPosition();
        if (FloatRect{0, 0, getSize().x, getSize().y}.contains(pos))
        {
            // Loop through the menus to check if the mouse is on top of them
            float menuWidth = 0;
            for (std::size_t i = 0; i < m_menus.size(); ++i)
            {
                menuWidth += m_menus[i].text.getSize().x + (2 * m_distanceToSideCached);
                if (pos.x >= menuWidth)
                    continue;

                // Close the menu when it was already open
                if (m_visibleMenu == static_cast<int>(i))
                    closeMenu();

                // If this menu can be opened then do so
                else if (m_menus[i].enabled && !m_menus[i].menuItems.empty())
                {
                    updateMenuTextColor(m_menus[i], true);
                    m_visibleMenu = static_cast<int>(i);
                }

                break;
            }
        }

        m_mouseDown = true;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void MenuBar::leftMouseReleased(Vector2f pos)
    {
        if (!m_mouseDown || (m_visibleMenu == -1))
            return;

        pos -= getPosition();

        // Check if the mouse is on top of one of the menus
        if (FloatRect{0, 0, getSize().x, getSize().y}.contains(pos))
            return;

        auto* menu = &m_menus[m_visibleMenu];
        std::vector<sf::String> hierarchy;
        hierarchy.push_back(m_menus[m_visibleMenu].text.getString());
        while (menu->selectedMenuItem != -1)
        {
            auto& menuItem = menu->menuItems[menu->selectedMenuItem];
            hierarchy.push_back(menuItem.text.getString());
            if (menuItem.menuItems.empty())
            {
                onMenuItemClick.emit(this, menuItem.text.getString(), hierarchy);
                closeMenu();
                break;
            }

            menu = &menuItem;
        }
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void MenuBar::mouseMoved(Vector2f pos)
    {
        if (!m_mouseHover)
            mouseEnteredWidget();

        // Don't open a menu without having clicked first
        if (m_visibleMenu == -1)
            return;

        // Check if the mouse is on top of the menu bar (not on an open menus)
        if (FloatRect{getPosition(), getSize()}.contains(pos))
        {
            pos -= getPosition();

            // Loop through the menus to check if the mouse is on top of them
            bool handled = false;
            float menuWidth = 0;
            for (std::size_t i = 0; i < m_menus.size(); ++i)
            {
                menuWidth += m_menus[i].text.getSize().x + (2 * m_distanceToSideCached);
                if (pos.x >= menuWidth)
                    continue;

                // Check if the menu is already open
                if (m_visibleMenu == static_cast<int>(i))
                {
                    // If one of the menu items is selected then unselect it
                    if (m_menus[m_visibleMenu].selectedMenuItem != -1)
                    {
                        updateMenuTextColor(m_menus[i].menuItems[m_menus[m_visibleMenu].selectedMenuItem], false);
                        m_menus[m_visibleMenu].selectedMenuItem = -1;
                    }
                }
                else // The menu isn't open yet
                {
                    // If there is another menu open then close it first
                    closeMenu();

                    // If this menu can be opened then do so
                    if (m_menus[i].enabled && !m_menus[i].menuItems.empty())
                    {
                        updateMenuTextColor(m_menus[i], true);
                        m_visibleMenu = static_cast<int>(i);
                    }
                }

                handled = true;
                break;
            }

            // The mouse is to the right of all menus, deselect the selected item of the deepest submenu
            if (!handled)
                deselectBottomItem();
        }
        else // The mouse is on top of the open menu
        {
            Vector2f menuPos = getPosition();
            for (int i = 0; i < m_visibleMenu; ++i)
                menuPos.x += m_menus[i].text.getSize().x + (2 * m_distanceToSideCached);

            if (m_invertedMenuDirection)
                menuPos.y -= getSize().y * m_menus[m_visibleMenu].menuItems.size();
            else
                menuPos.y += getSize().y;

            // The menu is moved to the left if it otherwise falls off the screen
            bool openSubMenuToRight = true;
            const float menuWidth = calculateMenuWidth(m_menus[m_visibleMenu]);
            if (getParent() && (menuPos.x + menuWidth > getParent()->getInnerSize().x))
            {
                menuPos.x = std::max(0.f, getParent()->getInnerSize().x - menuWidth);
                openSubMenuToRight = false;
            }

            Menu* menuBelowMouse;
            int menuItemIndexBelowMouse;
            if (findMenuItemBelowMouse(menuPos, pos, openSubMenuToRight, m_menus[m_visibleMenu], menuWidth, &menuBelowMouse, &menuItemIndexBelowMouse))
            {
                // Check if the mouse is on a different item than before
                auto& menu = *menuBelowMouse;
                if (menuItemIndexBelowMouse != menu.selectedMenuItem)
                {
                    // If another of the menu items is selected then unselect it
                    if (menu.selectedMenuItem != -1)
                        closeSubMenus(menu.menuItems, menu.selectedMenuItem);

                    // Mark the item below the mouse as selected
                    if (menu.menuItems[menuItemIndexBelowMouse].enabled)
                    {
                        updateMenuTextColor(menu.menuItems[menuItemIndexBelowMouse], true);
                        menu.selectedMenuItem = menuItemIndexBelowMouse;
                    }
                }
                else // We already selected this item
                {
                    // If the selected item has a submenu then unselect its item
                    if (menu.menuItems[menuItemIndexBelowMouse].selectedMenuItem != -1)
                        closeSubMenus(menu.menuItems[menuItemIndexBelowMouse].menuItems, menu.menuItems[menuItemIndexBelowMouse].selectedMenuItem);
                }
            }
        }
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void MenuBar::mouseNoLongerDown()
    {
        // Close the open menu, but not when it just opened because of this mouse click
        if (!m_mouseDown)
            closeMenu();

        Widget::mouseNoLongerDown();
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void MenuBar::mouseLeftWidget()
    {
        // Deselect the selected item of the deepest submenu
        if (m_visibleMenu != -1)
            deselectBottomItem();

        Widget::mouseLeftWidget();
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    Signal& MenuBar::getSignal(std::string signalName)
    {
        if (signalName == toLower(onMenuItemClick.getName()))
            return onMenuItemClick;
        else
            return Widget::getSignal(std::move(signalName));
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void MenuBar::rendererChanged(const std::string& property)
    {
        if (property == "textcolor")
        {
            m_textColorCached = getSharedRenderer()->getTextColor();
            updateTextColors(m_menus, m_visibleMenu);
        }
        else if (property == "selectedtextcolor")
        {
            m_selectedTextColorCached = getSharedRenderer()->getSelectedTextColor();
            updateTextColors(m_menus, m_visibleMenu);
        }
        else if (property == "textcolordisabled")
        {
            m_textColorDisabledCached = getSharedRenderer()->getTextColorDisabled();
            updateTextColors(m_menus, m_visibleMenu);
        }
        else if (property == "texturebackground")
        {
            m_spriteBackground.setTexture(getSharedRenderer()->getTextureBackground());
        }
        else if (property == "textureitembackground")
        {
            m_spriteItemBackground.setTexture(getSharedRenderer()->getTextureItemBackground());
        }
        else if (property == "textureselecteditembackground")
        {
            m_spriteSelectedItemBackground.setTexture(getSharedRenderer()->getTextureSelectedItemBackground());
        }
        else if (property == "backgroundcolor")
        {
            m_backgroundColorCached = getSharedRenderer()->getBackgroundColor();
        }
        else if (property == "selectedbackgroundcolor")
        {
            m_selectedBackgroundColorCached = getSharedRenderer()->getSelectedBackgroundColor();
        }
        else if (property == "distancetoside")
        {
            m_distanceToSideCached = getSharedRenderer()->getDistanceToSide();
        }
        else if (property == "opacity")
        {
            Widget::rendererChanged(property);
            updateTextOpacity(m_menus);
            m_spriteBackground.setOpacity(m_opacityCached);
        }
        else if (property == "font")
        {
            Widget::rendererChanged(property);
            updateTextFont(m_menus);
        }
        else
            Widget::rendererChanged(property);
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    std::unique_ptr<DataIO::Node> MenuBar::save(SavingRenderersMap& renderers) const
    {
        auto node = Widget::save(renderers);

        saveMenus(node, m_menus);

        node->propertyValuePairs["TextSize"] = std::make_unique<DataIO::ValueNode>(to_string(m_textSize));
        node->propertyValuePairs["MinimumSubMenuWidth"] = std::make_unique<DataIO::ValueNode>(to_string(m_minimumSubMenuWidth));
        if (m_invertedMenuDirection)
            node->propertyValuePairs["InvertedMenuDirection"] = std::make_unique<DataIO::ValueNode>("true");

        return node;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void MenuBar::load(const std::unique_ptr<DataIO::Node>& node, const LoadingRenderersMap& renderers)
    {
        Widget::load(node, renderers);

        if (node->propertyValuePairs["textsize"])
            setTextSize(tgui::stoi(node->propertyValuePairs["textsize"]->value));
        if (node->propertyValuePairs["minimumsubmenuwidth"])
            setMinimumSubMenuWidth(tgui::stof(node->propertyValuePairs["minimumsubmenuwidth"]->value));
        if (node->propertyValuePairs["invertedmenudirection"])
            setInvertedMenuDirection(tgui::Deserializer::deserialize(tgui::ObjectConverter::Type::Bool, node->propertyValuePairs["invertedmenudirection"]->value).getBool());

        loadMenus(node, m_menus);

        // Remove the 'menu' nodes as they have been processed
        node->children.erase(std::remove_if(node->children.begin(), node->children.end(),
            [](const std::unique_ptr<DataIO::Node>& child){ return toLower(child->name) == "menu"; }), node->children.end());

        // Update the text colors to properly display disabled menus
        updateTextColors(m_menus, m_visibleMenu);
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void MenuBar::draw(sf::RenderTarget& target, sf::RenderStates states) const
    {
        states.transform.translate(getPosition());

        // Draw the background
        if (m_spriteBackground.isSet())
            m_spriteBackground.draw(target, states);
        else
            drawRectangleShape(target, states, getSize(), m_backgroundColorCached);

        if (m_menus.empty())
            return;

        Sprite backgroundSprite = m_spriteItemBackground;
        drawMenusOnBar(target, states, backgroundSprite);

        // Draw the menu if one is opened
        if (m_visibleMenu >= 0)
        {
            // Find the position of the menu
            float leftOffset = 0;
            for (int i = 0; i < m_visibleMenu; ++i)
                leftOffset += m_menus[i].text.getSize().x + (2 * m_distanceToSideCached);

            // Move the menu to the left if it otherwise falls off the screen
            bool openSubMenuToRight = true;
            const float menuWidth = calculateMenuWidth(m_menus[m_visibleMenu]);
            if (getParent() && (getPosition().x + leftOffset + menuWidth > getParent()->getInnerSize().x))
            {
                leftOffset = std::max(0.f, getParent()->getInnerSize().x - menuWidth);
                openSubMenuToRight = false;
            }

            if (m_invertedMenuDirection)
                states.transform.translate({leftOffset, -getSize().y * m_menus[m_visibleMenu].menuItems.size()});
            else
                states.transform.translate({leftOffset, getSize().y});

            drawMenu(target, states, m_menus[m_visibleMenu], menuWidth, backgroundSprite, getPosition().x + leftOffset, openSubMenuToRight);
        }
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void MenuBar::createMenu(std::vector<Menu>& menus, const sf::String& text)
    {
        Menu newMenu;
        newMenu.text.setFont(m_fontCached);
        newMenu.text.setColor(m_textColorCached);
        newMenu.text.setOpacity(m_opacityCached);
        newMenu.text.setCharacterSize(m_textSize);
        newMenu.text.setString(text);
        menus.push_back(std::move(newMenu));
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    MenuBar::Menu* MenuBar::findMenu(const std::vector<sf::String>& hierarchy, unsigned int parentIndex, std::vector<Menu>& menus, bool createParents)
    {
        for (auto& menu : menus)
        {
            if (menu.text.getString() != hierarchy[parentIndex])
                continue;

            if (parentIndex + 2 == hierarchy.size())
                return &menu;
            else
                return findMenu(hierarchy, parentIndex + 1, menu.menuItems, createParents);
        }

        if (createParents)
        {
            createMenu(menus, hierarchy[parentIndex]);
            if (parentIndex + 2 == hierarchy.size())
                return &menus.back();
            else
                return findMenu(hierarchy, parentIndex + 1, menus.back().menuItems, createParents);
        }

        return nullptr;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    const MenuBar::Menu* MenuBar::findMenu(const std::vector<sf::String>& hierarchy, unsigned int parentIndex, const std::vector<Menu>& menus) const
    {
        for (auto& menu : menus)
        {
            if (menu.text.getString() != hierarchy[parentIndex])
                continue;

            if (parentIndex + 2 == hierarchy.size())
                return &menu;
            else
                return findMenu(hierarchy, parentIndex + 1, menu.menuItems);
        }

        return nullptr;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    const MenuBar::Menu* MenuBar::findMenuItem(const std::vector<sf::String>& hierarchy) const
    {
        if (hierarchy.size() < 2)
            return nullptr;

        const auto* menu = findMenu(hierarchy, 0, m_menus);
        if (!menu)
            return nullptr;

        for (auto& menuItem : menu->menuItems)
        {
            if (menuItem.text.getString() != hierarchy.back())
                continue;

            return &menuItem;
        }

        return nullptr;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void MenuBar::loadMenus(const std::unique_ptr<DataIO::Node>& node, std::vector<Menu>& menus)
    {
        for (const auto& childNode : node->children)
        {
            if (toLower(childNode->name) != "menu")
                continue;

            // Every menu node should either have a Name (TGUI 0.8.0) or a Text (TGUI >= 0.8.1) property
        #ifdef TGUI_REMOVE_DEPRECATED_CODE
            if (!childNode->propertyValuePairs["text"])
                throw Exception{"Failed to parse 'Menu' property, expected a nested 'Text' propery"};

            const sf::String menuText = Deserializer::deserialize(ObjectConverter::Type::String, childNode->propertyValuePairs["text"]->value).getString();
            createMenu(menus, menuText);
        #else
            if (!childNode->propertyValuePairs["name"] && !childNode->propertyValuePairs["text"])
                throw Exception{"Failed to parse 'Menu' property, expected a nested 'Text' propery"};

            const sf::String menuText = Deserializer::deserialize(ObjectConverter::Type::String,
                childNode->propertyValuePairs[childNode->propertyValuePairs["text"] ? "text" : "name"]->value).getString();
            createMenu(menus, menuText);
        #endif

            if (childNode->propertyValuePairs["enabled"])
                menus.back().enabled = Deserializer::deserialize(ObjectConverter::Type::Bool, childNode->propertyValuePairs["enabled"]->value).getBool();

            // Recursively handle the menu nodes
            if (!childNode->children.empty())
                loadMenus(childNode, menus.back().menuItems);

            // Menu items can also be stored in an string array in the 'Items' property instead of as a nested Menu section
            if (childNode->propertyValuePairs["items"])
            {
                if (!childNode->propertyValuePairs["items"]->listNode)
                    throw Exception{"Failed to parse 'Items' property inside 'Menu' property, expected a list as value"};

            #ifndef TGUI_REMOVE_DEPRECATED_CODE
                // The ItemsEnabled property existed in TGUI 0.8.0 but was replaced with nested Menu sections in TGUI 0.8.1
                if (childNode->propertyValuePairs["itemsenabled"])
                {
                    if (!childNode->propertyValuePairs["itemsenabled"]->listNode)
                        throw Exception{"Failed to parse 'ItemsEnabled' property inside 'Menu' property, expected a list as value"};
                    if (childNode->propertyValuePairs["items"]->valueList.size() != childNode->propertyValuePairs["itemsenabled"]->valueList.size())
                        throw Exception{"Failed to parse 'ItemsEnabled' property inside 'Menu' property, length differs from 'Items' propery"};
                }
            #endif

                for (std::size_t i = 0; i < childNode->propertyValuePairs["items"]->valueList.size(); ++i)
                {
                    const sf::String menuItemText = Deserializer::deserialize(ObjectConverter::Type::String, childNode->propertyValuePairs["items"]->valueList[i]).getString();
                    createMenu(menus.back().menuItems, menuItemText);

                #ifndef TGUI_REMOVE_DEPRECATED_CODE
                    if (childNode->propertyValuePairs["itemsenabled"])
                        menus.back().menuItems.back().enabled = Deserializer::deserialize(ObjectConverter::Type::Bool, childNode->propertyValuePairs["itemsenabled"]->valueList[i]).getBool();
                #endif
                }
            }
        }
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void MenuBar::closeSubMenus(std::vector<Menu>& menus, int& selectedMenu)
    {
        if (menus[selectedMenu].selectedMenuItem != -1)
            closeSubMenus(menus[selectedMenu].menuItems, menus[selectedMenu].selectedMenuItem);

        updateMenuTextColor(menus[selectedMenu], false);
        selectedMenu = -1;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void MenuBar::deselectBottomItem()
    {
        auto* menu = &m_menus[m_visibleMenu];
        while (menu->selectedMenuItem != -1)
        {
            auto& menuItem = menu->menuItems[menu->selectedMenuItem];
            if (menuItem.menuItems.empty())
            {
                closeSubMenus(menu->menuItems, menu->selectedMenuItem);
                break;
            }

            menu = &menuItem;
        }
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void MenuBar::updateMenuTextColor(Menu& menu, bool selected)
    {
        if ((!m_enabled || !menu.enabled) && m_textColorDisabledCached.isSet())
            menu.text.setColor(m_textColorDisabledCached);
        else if (selected && m_selectedTextColorCached.isSet())
            menu.text.setColor(m_selectedTextColorCached);
        else
            menu.text.setColor(m_textColorCached);
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void MenuBar::updateTextColors(std::vector<Menu>& menus, int selectedMenu)
    {
        for (std::size_t i = 0; i < menus.size(); ++i)
        {
            updateMenuTextColor(menus[i], (selectedMenu == static_cast<int>(i)));
            updateTextColors(menus[i].menuItems, menus[i].selectedMenuItem);
        }
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void MenuBar::updateTextOpacity(std::vector<Menu>& menus)
    {
        for (auto& menu : menus)
        {
            menu.text.setOpacity(m_opacityCached);
            updateTextOpacity(menu.menuItems);
        }
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void MenuBar::updateTextFont(std::vector<Menu>& menus)
    {
        for (auto& menu : menus)
        {
            menu.text.setFont(m_fontCached);
            updateTextFont(menu.menuItems);
        }
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    float MenuBar::calculateMenuWidth(const Menu& menu) const
    {
        float maxWidth = m_minimumSubMenuWidth;
        for (const auto& item : menu.menuItems)
        {
            float width = item.text.getSize().x + (2.f * m_distanceToSideCached);

            // Reserve space for an arrow if there are submenus
            if (!item.menuItems.empty())
                width += (getSize().y / 4.f) + m_distanceToSideCached;

            if (width > maxWidth)
                maxWidth = width;
        }

        return maxWidth;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    Vector2f MenuBar::calculateSubmenuOffset(const Menu& menu, float globalLeftPos, float menuWidth, float subMenuWidth, bool& openSubMenuToRight) const
    {
        float leftOffset;
        if (openSubMenuToRight)
        {
            leftOffset = menuWidth;
            if (getParent() && (globalLeftPos + leftOffset + subMenuWidth > getParent()->getInnerSize().x))
            {
                if (globalLeftPos + leftOffset + subMenuWidth - getParent()->getInnerSize().x < globalLeftPos)
                {
                    leftOffset = -subMenuWidth;
                    openSubMenuToRight = false;
                }
            }
        }
        else // Submenu opens to the left side
        {
            leftOffset = -subMenuWidth;
            if (getParent() && (globalLeftPos < subMenuWidth))
            {
                if (getParent()->getInnerSize().x - menuWidth - globalLeftPos > globalLeftPos)
                {
                    leftOffset = menuWidth;
                    openSubMenuToRight = true;
                }
            }
        }

        float topOffset = getSize().y * menu.selectedMenuItem;
        if (m_invertedMenuDirection)
            topOffset -= getSize().y * (menu.menuItems[menu.selectedMenuItem].menuItems.size() - 1);

        return {leftOffset, topOffset};
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    bool MenuBar::isMouseOnTopOfMenu(Vector2f menuPos, Vector2f mousePos, bool openSubMenuToRight, const Menu& menu, float menuWidth) const
    {
        // Check if the mouse is on top of the menu
        if (FloatRect{menuPos.x, menuPos.y, menuWidth, menu.menuItems.size() * getSize().y}.contains(mousePos))
            return true;

        // Check if the mouse is on one of the submenus
        if ((menu.selectedMenuItem >= 0) && !menu.menuItems[menu.selectedMenuItem].menuItems.empty())
        {
            const float subMenuWidth = calculateMenuWidth(menu.menuItems[menu.selectedMenuItem]);
            const Vector2f offset = calculateSubmenuOffset(menu, menuPos.x, menuWidth, subMenuWidth, openSubMenuToRight);
            if (isMouseOnTopOfMenu(menuPos + offset, mousePos, openSubMenuToRight, menu.menuItems[menu.selectedMenuItem], subMenuWidth))
                return true;
        }

        return false;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    bool MenuBar::findMenuItemBelowMouse(Vector2f menuPos, Vector2f mousePos, bool openSubMenuToRight, Menu& menu, float menuWidth, Menu** resultMenu, int* resultSelectedMenuItem)
    {
        // Loop over the open submenus and make sure to handle them first as menus can overlap
        if ((menu.selectedMenuItem >= 0) && !menu.menuItems[menu.selectedMenuItem].menuItems.empty())
        {
            const float subMenuWidth = calculateMenuWidth(menu.menuItems[menu.selectedMenuItem]);
            const Vector2f offset = calculateSubmenuOffset(menu, menuPos.x, menuWidth, subMenuWidth, openSubMenuToRight);
            if (findMenuItemBelowMouse(menuPos + offset, mousePos, openSubMenuToRight, menu.menuItems[menu.selectedMenuItem], subMenuWidth, resultMenu, resultSelectedMenuItem))
                return true;
        }

        // Check if the mouse is on top of the menu
        if (FloatRect{menuPos.x, menuPos.y, menuWidth, menu.menuItems.size() * getSize().y}.contains(mousePos))
        {
            *resultMenu = &menu;
            *resultSelectedMenuItem = static_cast<int>((mousePos.y - menuPos.y) / getSize().y);
            return true;
        }

        return false;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void MenuBar::drawMenusOnBar(sf::RenderTarget& target, sf::RenderStates states, Sprite& backgroundSprite) const
    {
        sf::Transform oldTransform = states.transform;

        // Draw the backgrounds
        for (std::size_t i = 0; i < m_menus.size(); ++i)
        {
            const bool isMenuOpen = (m_visibleMenu == static_cast<int>(i));
            const float width = m_menus[i].text.getSize().x + (2 * m_distanceToSideCached);
            if (backgroundSprite.isSet())
            {
                if (isMenuOpen && m_spriteSelectedItemBackground.isSet())
                {
                    Sprite selectedBackgroundSprite = m_spriteSelectedItemBackground;
                    selectedBackgroundSprite.setSize({width, getSize().y});
                    selectedBackgroundSprite.draw(target, states);
                }
                else // Not selected or no different texture for selected menu
                {
                    backgroundSprite.setSize({width, getSize().y});
                    backgroundSprite.draw(target, states);
                }
            }
            else // No textures where loaded
            {
                if (isMenuOpen && m_selectedBackgroundColorCached.isSet())
                    drawRectangleShape(target, states, {width, getSize().y}, m_selectedBackgroundColorCached);
            }

            states.transform.translate({width, 0});
        }

        states.transform = oldTransform;

        // Draw the texts
        const float textHeight = m_menus[0].text.getSize().y;
        states.transform.translate({m_distanceToSideCached, (getSize().y - textHeight) / 2.f});
        for (std::size_t i = 0; i < m_menus.size(); ++i)
        {
            m_menus[i].text.draw(target, states);

            const float width = m_menus[i].text.getSize().x + (2 * m_distanceToSideCached);
            states.transform.translate({width, 0});
        }
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void MenuBar::drawMenu(sf::RenderTarget& target, sf::RenderStates states, const Menu& menu, float menuWidth, Sprite& backgroundSprite, float globalLeftPos, bool openSubMenuToRight) const
    {
        if (menu.menuItems.empty())
            return;

        sf::Transform oldTransform = states.transform;

        // Draw the backgrounds
        if ((menu.selectedMenuItem == -1) && !backgroundSprite.isSet() && !m_selectedBackgroundColorCached.isSet())
        {
            drawRectangleShape(target, states, {menuWidth, getSize().y * menu.menuItems.size()}, m_backgroundColorCached);
        }
        else // We can't draw the entire menu with a singe draw call
        {
            for (std::size_t j = 0; j < menu.menuItems.size(); ++j)
            {
                const bool isMenuItemSelected = (menu.selectedMenuItem == static_cast<int>(j));
                if (backgroundSprite.isSet())
                {
                    if (isMenuItemSelected && m_spriteSelectedItemBackground.isSet())
                    {
                        Sprite selectedBackgroundSprite = m_spriteSelectedItemBackground;
                        selectedBackgroundSprite.setSize({menuWidth, getSize().y});
                        selectedBackgroundSprite.draw(target, states);
                    }
                    else // Not selected or no different texture for selected menu
                    {
                        backgroundSprite.setSize({menuWidth, getSize().y});
                        backgroundSprite.draw(target, states);
                    }
                }
                else // No textures where loaded
                {
                    if (isMenuItemSelected && m_selectedBackgroundColorCached.isSet())
                        drawRectangleShape(target, states, {menuWidth, getSize().y}, m_selectedBackgroundColorCached);
                    else
                        drawRectangleShape(target, states, {menuWidth, getSize().y}, m_backgroundColorCached);
                }

                states.transform.translate({0, getSize().y});
            }

            states.transform = oldTransform;
        }

        // Draw the texts (and arrows when there are submenus)
        states.transform.translate({m_distanceToSideCached, (getSize().y - menu.text.getSize().y) / 2.f});
        for (std::size_t j = 0; j < menu.menuItems.size(); ++j)
        {
            menu.menuItems[j].text.draw(target, states);

            // Draw an arrow next to the text if there is a submenu
            if (!menu.menuItems[j].menuItems.empty())
            {
                sf::Transform textTransform = states.transform;
                const float arrowHeight = getSize().y / 2.f;
                const float arrowWidth = arrowHeight / 2.f;
                states.transform.translate({menuWidth - 2*m_distanceToSideCached - arrowWidth, // 2x m_distanceToSideCached because we already translated once
                                            (menu.menuItems[j].text.getSize().y - arrowHeight) / 2.f});

                sf::ConvexShape arrow{3};
                arrow.setPoint(0, {0, 0});
                arrow.setPoint(1, {arrowWidth, arrowHeight / 2.f});
                arrow.setPoint(2, {0, arrowHeight});

                if ((!m_enabled || !menu.menuItems[j].enabled) && m_textColorDisabledCached.isSet())
                    arrow.setFillColor(Color::calcColorOpacity(m_textColorDisabledCached, m_opacityCached));
                else if ((menu.selectedMenuItem == static_cast<int>(j)) && m_selectedTextColorCached.isSet())
                    arrow.setFillColor(Color::calcColorOpacity(m_selectedTextColorCached, m_opacityCached));
                else
                    arrow.setFillColor(Color::calcColorOpacity(m_textColorCached, m_opacityCached));

                target.draw(arrow, states);
                states.transform = textTransform;
            }

            states.transform.translate({0, getSize().y});
        }

        // Draw the submenu if one is opened
        if ((menu.selectedMenuItem >= 0) && !menu.menuItems[menu.selectedMenuItem].menuItems.empty())
        {
            states.transform = oldTransform;

            // If there isn't enough room on the right side then open the menu to the left if it has more room
            const float subMenuWidth = calculateMenuWidth(menu.menuItems[menu.selectedMenuItem]);
            float leftOffset;
            if (openSubMenuToRight)
            {
                leftOffset = menuWidth;
                if (getParent() && (globalLeftPos + leftOffset + subMenuWidth > getParent()->getInnerSize().x))
                {
                    if (globalLeftPos + leftOffset + subMenuWidth - getParent()->getInnerSize().x < globalLeftPos)
                    {
                        leftOffset = -subMenuWidth;
                        openSubMenuToRight = false;
                    }
                }
            }
            else // Submenu opens to the left side
            {
                leftOffset = -subMenuWidth;
                if (getParent() && (globalLeftPos < subMenuWidth))
                {
                    if (getParent()->getInnerSize().x - menuWidth - globalLeftPos > globalLeftPos)
                    {
                        leftOffset = menuWidth;
                        openSubMenuToRight = true;
                    }
                }
            }

            float topOffset = getSize().y * menu.selectedMenuItem;
            if (m_invertedMenuDirection)
                topOffset -= getSize().y * (menu.menuItems[menu.selectedMenuItem].menuItems.size() - 1);

            states.transform.translate({leftOffset, topOffset});
            drawMenu(target, states, menu.menuItems[menu.selectedMenuItem], subMenuWidth, backgroundSprite, globalLeftPos + leftOffset, openSubMenuToRight);
        }
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// TGUI - Texus' Graphical User Interface
// Copyright (C) 2012-2018 Bruno Van de Velde (vdv_b@tgui.eu)
//
// This software is provided 'as-is', without any express or implied warranty.
// In no event will the authors be held liable for any damages arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it freely,
// subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented;
//    you must not claim that you wrote the original software.
//    If you use this software in a product, an acknowledgment
//    in the product documentation would be appreciated but is not required.
//
// 2. Altered source versions must be plainly marked as such,
//    and must not be misrepresented as being the original software.
//
// 3. This notice may not be removed or altered from any source distribution.
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


#include <TGUI/Widgets/Picture.hpp>

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace tgui
{
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    Picture::Picture()
    {
        m_type = "Picture";

        m_renderer = aurora::makeCopied<PictureRenderer>();
        setRenderer(Theme::getDefault()->getRendererNoThrow(m_type));
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    Picture::Picture(const Texture& texture, bool transparentTexture) :
        Picture{}
    {
        getRenderer()->setTexture(texture);
        getRenderer()->setTransparentTexture(transparentTexture);
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    Picture::Ptr Picture::create(const Texture& texture, bool fullyClickable)
    {
        return std::make_shared<Picture>(texture, fullyClickable);
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    Picture::Ptr Picture::copy(Picture::ConstPtr picture)
    {
        if (picture)
            return std::static_pointer_cast<Picture>(picture->clone());
        else
            return nullptr;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    PictureRenderer* Picture::getSharedRenderer()
    {
        return aurora::downcast<PictureRenderer*>(Widget::getSharedRenderer());
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    const PictureRenderer* Picture::getSharedRenderer() const
    {
        return aurora::downcast<const PictureRenderer*>(Widget::getSharedRenderer());
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    PictureRenderer* Picture::getRenderer()
    {
        return aurora::downcast<PictureRenderer*>(Widget::getRenderer());
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    const PictureRenderer* Picture::getRenderer() const
    {
        return aurora::downcast<const PictureRenderer*>(Widget::getRenderer());
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void Picture::setSize(const Layout2d& size)
    {
        Widget::setSize(size);

        m_sprite.setSize(getSize());
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void Picture::ignoreMouseEvents(bool ignore)
    {
        m_ignoringMouseEvents = ignore;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    bool Picture::isIgnoringMouseEvents() const
    {
        return m_ignoringMouseEvents;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    bool Picture::canGainFocus() const
    {
        return false;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    bool Picture::mouseOnWidget(Vector2f pos) const
    {
        pos -= getPosition();

        // Check if the mouse is on top of the picture
        if (!m_ignoringMouseEvents && (FloatRect{0, 0, getSize().x, getSize().y}.contains(pos)))
        {
            if (!m_transparentTextureCached || !m_sprite.isTransparentPixel(pos))
                return true;
        }

        return false;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void Picture::leftMouseReleased(Vector2f pos)
    {
        const bool mouseDown = m_mouseDown;

        ClickableWidget::leftMouseReleased(pos);

        if (mouseDown)
        {
            // Check if you double-clicked
            if (m_possibleDoubleClick)
            {
                m_possibleDoubleClick = false;
                onDoubleClick.emit(this, pos - getPosition());
            }
            else // This is the first click
            {
                m_animationTimeElapsed = {};
                m_possibleDoubleClick = true;
            }
        }
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    Signal& Picture::getSignal(std::string signalName)
    {
        if (signalName == toLower(onDoubleClick.getName()))
            return onDoubleClick;
        else
            return ClickableWidget::getSignal(std::move(signalName));
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void Picture::rendererChanged(const std::string& property)
    {
        if (property == "texture")
        {
            const auto& texture = getSharedRenderer()->getTexture();

            if (!m_sprite.isSet() && (getSize() == Vector2f{0,0}))
                setSize(texture.getImageSize());

            m_sprite.setTexture(texture);
        }
        else if (property == "opacity")
        {
            Widget::rendererChanged(property);
            m_sprite.setOpacity(m_opacityCached);
        }
        else
            Widget::rendererChanged(property);
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    std::unique_ptr<DataIO::Node> Picture::save(SavingRenderersMap& renderers) const
    {
        auto node = Widget::save(renderers);

        if (m_ignoringMouseEvents)
            node->propertyValuePairs["IgnoreMouseEvents"] = std::make_unique<DataIO::ValueNode>(Serializer::serialize(m_ignoringMouseEvents));

        return node;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void Picture::load(const std::unique_ptr<DataIO::Node>& node, const LoadingRenderersMap& renderers)
    {
        Widget::load(node, renderers);

        if (node->propertyValuePairs["ignoremouseevents"])
            ignoreMouseEvents(Deserializer::deserialize(ObjectConverter::Type::Bool, node->propertyValuePairs["ignoremouseevents"]->value).getBool());
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void Picture::update(sf::Time elapsedTime)
    {
        Widget::update(elapsedTime);

        if (m_animationTimeElapsed >= sf::milliseconds(getDoubleClickTime()))
        {
            m_animationTimeElapsed = {};
            m_possibleDoubleClick = false;
        }
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void Picture::draw(sf::RenderTarget& target, sf::RenderStates states) const
    {
        states.transform.translate(getPosition());
        m_sprite.draw(target, states);
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// TGUI - Texus' Graphical User Interface
// Copyright (C) 2012-2018 Bruno Van de Velde (vdv_b@tgui.eu)
//
// This software is provided 'as-is', without any express or implied warranty.
// In no event will the authors be held liable for any damages arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it freely,
// subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented;
//    you must not claim that you wrote the original software.
//    If you use this software in a product, an acknowledgment
//    in the product documentation would be appreciated but is not required.
//
// 2. Altered source versions must be plainly marked as such,
//    and must not be misrepresented as being the original software.
//
// 3. This notice may not be removed or altered from any source distribution.
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


#include <TGUI/Widgets/RadioButtonGroup.hpp>

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace tgui
{
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    RadioButtonGroup::RadioButtonGroup()
    {
        m_type = "RadioButtonGroup";
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    RadioButtonGroup::Ptr RadioButtonGroup::create()
    {
        return std::make_shared<RadioButtonGroup>();
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    RadioButtonGroup::Ptr RadioButtonGroup::copy(RadioButtonGroup::ConstPtr group)
    {
        if (group)
            return std::static_pointer_cast<RadioButtonGroup>(group->clone());
        else
            return nullptr;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void RadioButtonGroup::draw(sf::RenderTarget& target, sf::RenderStates states) const
    {
        states.transform.translate(getPosition());
        drawWidgetContainer(&target, states);
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// TGUI - Texus' Graphical User Interface
// Copyright (C) 2012-2018 Bruno Van de Velde (vdv_b@tgui.eu)
//
// This software is provided 'as-is', without any express or implied warranty.
// In no event will the authors be held liable for any damages arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it freely,
// subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented;
//    you must not claim that you wrote the original software.
//    If you use this software in a product, an acknowledgment
//    in the product documentation would be appreciated but is not required.
//
// 2. Altered source versions must be plainly marked as such,
//    and must not be misrepresented as being the original software.
//
// 3. This notice may not be removed or altered from any source distribution.
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


#include <TGUI/Widgets/Scrollbar.hpp>
#include <SFML/Graphics/ConvexShape.hpp>

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace tgui
{
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    Scrollbar::Scrollbar()
    {
        m_type = "Scrollbar";

        m_draggableWidget = true;

        m_renderer = aurora::makeCopied<ScrollbarRenderer>();
        setRenderer(Theme::getDefault()->getRendererNoThrow(m_type));

        setSize(getDefaultWidth(), 160);
        m_sizeSet = false;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    Scrollbar::Ptr Scrollbar::create()
    {
        return std::make_shared<Scrollbar>();
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    Scrollbar::Ptr Scrollbar::copy(Scrollbar::ConstPtr scrollbar)
    {
        if (scrollbar)
            return std::static_pointer_cast<Scrollbar>(scrollbar->clone());
        else
            return nullptr;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    ScrollbarRenderer* Scrollbar::getSharedRenderer()
    {
        return aurora::downcast<ScrollbarRenderer*>(Widget::getSharedRenderer());
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    const ScrollbarRenderer* Scrollbar::getSharedRenderer() const
    {
        return aurora::downcast<const ScrollbarRenderer*>(Widget::getSharedRenderer());
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    ScrollbarRenderer* Scrollbar::getRenderer()
    {
        return aurora::downcast<ScrollbarRenderer*>(Widget::getRenderer());
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    const ScrollbarRenderer* Scrollbar::getRenderer() const
    {
        return aurora::downcast<const ScrollbarRenderer*>(Widget::getRenderer());
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void Scrollbar::setSize(const Layout2d& size)
    {
        Widget::setSize(size);

        m_sizeSet = true;
        updateSize();
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void Scrollbar::setMaximum(unsigned int maximum)
    {
        // Set the new maximum
        if (maximum > 0)
            m_maximum = maximum;
        else
            m_maximum = 1;

        // When the value is above the maximum then adjust it
        if (m_maximum < m_viewportSize)
            setValue(0);
        else if (m_value > m_maximum - m_viewportSize)
            setValue(m_maximum - m_viewportSize);

        // Recalculate the size and position of the thumb image
        updateSize();
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    unsigned int Scrollbar::getMaximum() const
    {
        return m_maximum;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void Scrollbar::setValue(unsigned int value)
    {
        // When the value is above the maximum then adjust it
        if (m_maximum < m_viewportSize)
            value = 0;
        else if (value > m_maximum - m_viewportSize)
            value = m_maximum - m_viewportSize;

        if (m_value != value)
        {
            m_value = value;

            onValueChange.emit(this, m_value);

            // Recalculate the size and position of the thumb image
            updateSize();
        }
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    unsigned int Scrollbar::getValue() const
    {
        return m_value;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void Scrollbar::setViewportSize(unsigned int viewportSize)
    {
        // Set the new value
        m_viewportSize = viewportSize;

        // When the value is above the maximum then adjust it
        if (m_maximum < m_viewportSize)
            setValue(0);
        else if (m_value > m_maximum - m_viewportSize)
            setValue(m_maximum - m_viewportSize);

        // Recalculate the size and position of the thumb image
        updateSize();
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    unsigned int Scrollbar::getViewportSize() const
    {
        return m_viewportSize;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void Scrollbar::setScrollAmount(unsigned int scrollAmount)
    {
        m_scrollAmount = scrollAmount;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    unsigned int Scrollbar::getScrollAmount() const
    {
        return m_scrollAmount;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void Scrollbar::setAutoHide(bool autoHide)
    {
        m_autoHide = autoHide;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    bool Scrollbar::getAutoHide() const
    {
        return m_autoHide;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    float Scrollbar::getDefaultWidth()
    {
        if (m_spriteTrack.isSet())
            return m_spriteTrack.getTexture().getImageSize().x;
        else
            return 16;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    bool Scrollbar::mouseOnWidget(Vector2f pos) const
    {
        // Don't make any calculations when no scrollbar is needed
        if (m_autoHide && (m_maximum <= m_viewportSize))
            return false;

        pos -= getPosition();
        if (FloatRect{0, 0, getSize().x, getSize().y}.contains(pos))
        {
            if (!m_transparentTextureCached)
                return true;

            if (!m_spriteArrowUp.isTransparentPixel(pos))
                return true;
            if (!m_spriteArrowDown.isTransparentPixel(pos - m_arrowDown.getPosition()))
                return true;
            if (!m_spriteTrack.isTransparentPixel(pos - m_track.getPosition()))
                return true;
        }

        return false;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void Scrollbar::leftMousePressed(Vector2f pos)
    {
        pos -= getPosition();

        m_mouseDown = true;
        m_mouseDownOnArrow = false;

        if (m_verticalScroll)
        {
            // Check if the arrows are drawn at full size
            if (getSize().y > m_arrowUp.height + m_arrowDown.height)
            {
                // Check if you clicked on one of the arrows
                if ((pos.y < m_arrowUp.height) || (pos.y >= getSize().y - m_arrowUp.height))
                    m_mouseDownOnArrow = true;
            }
            else // The arrows are not drawn at full size (there is no track)
                m_mouseDownOnArrow = true;
        }
        else
        {
            // Check if the arrows are drawn at full size
            if (getSize().x > m_arrowUp.height + m_arrowDown.height)
            {
                // Check if you clicked on one of the arrows
                if ((pos.x < m_arrowUp.height) || (pos.x >= getSize().x - m_arrowUp.height))
                    m_mouseDownOnArrow = true;
            }
            else // The arrows are not drawn at full size (there is no track)
                m_mouseDownOnArrow = true;
        }

        // Check if the mouse is on top of the thumb
        if (FloatRect(m_thumb.left, m_thumb.top, m_thumb.width, m_thumb.height).contains(pos))
        {
            m_mouseDownOnThumbPos.x = pos.x - m_thumb.left;
            m_mouseDownOnThumbPos.y = pos.y - m_thumb.top;

            m_mouseDownOnThumb = true;
        }
        else // The mouse is not on top of the thumb
            m_mouseDownOnThumb = false;

        // Refresh the scrollbar value
        if (!m_mouseDownOnArrow)
            mouseMoved(pos + getPosition());
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void Scrollbar::leftMouseReleased(Vector2f pos)
    {
        // Check if one of the arrows was clicked
        if (m_mouseDown && m_mouseDownOnArrow)
        {
            // Only continue when the calculations can be made
            if (m_maximum > m_viewportSize)
            {
                bool valueDown = false;
                bool valueUp = false;

                pos -= getPosition();

                // Check in which direction the scrollbar lies
                if (m_verticalScroll)
                {
                    // Check if the arrows are drawn at full size
                    if (getSize().y > m_arrowUp.height + m_arrowDown.height)
                    {
                        // Check if you clicked on the top arrow
                        if (pos.y < m_arrowUp.height)
                            valueDown = true;

                        // Check if you clicked the down arrow
                        else if (pos.y >= getSize().y - m_arrowUp.height)
                            valueUp = true;
                    }
                    else // The arrows are not drawn at full size
                    {
                        // Check on which arrow you clicked
                        if (pos.y < getSize().y * 0.5f)
                            valueDown = true;
                        else // You clicked on the bottom arrow
                            valueUp = true;
                    }
                }
                else // the scrollbar lies horizontal
                {
                    // Check if the arrows are drawn at full size
                    if (getSize().x > m_arrowUp.height + m_arrowDown.height)
                    {
                        // Check if you clicked on the top arrow
                        if (pos.x < m_arrowUp.height)
                            valueDown = true;

                        // Check if you clicked the down arrow
                        else if (pos.x >= getSize().x - m_arrowUp.height)
                            valueUp = true;
                    }
                    else // The arrows are not drawn at full size
                    {
                        // Check on which arrow you clicked
                        if (pos.x < getSize().x * 0.5f)
                            valueDown = true;
                        else // You clicked on the bottom arrow
                            valueUp = true;
                    }
                }

                if (valueDown)
                {
                    if (m_value > m_scrollAmount)
                    {
                        if (m_value % m_scrollAmount)
                            setValue(m_value - (m_value % m_scrollAmount));
                        else
                            setValue(m_value - m_scrollAmount);
                    }
                    else
                        setValue(0);
                }
                else if (valueUp)
                {
                    if (m_value + m_scrollAmount < m_maximum - m_viewportSize + 1)
                    {
                        if (m_value % m_scrollAmount)
                            setValue(m_value + (m_scrollAmount - (m_value % m_scrollAmount)));
                        else
                            setValue(m_value + m_scrollAmount);
                    }
                    else
                        setValue(m_maximum - m_viewportSize);
                }
            }
        }

        // The thumb might have been dragged between two values
        if (m_mouseDown)
            updateThumbPosition();
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void Scrollbar::mouseMoved(Vector2f pos)
    {
        pos -= getPosition();

        if (!m_mouseHover)
            mouseEnteredWidget();

        // Check if the mouse button went down on top of the track (or thumb)
        if (m_mouseDown && !m_mouseDownOnArrow)
        {
            // Don't continue if the calculations can't be made
            if (!m_autoHide && (m_maximum <= m_viewportSize))
                return;

            // Check in which direction the scrollbar lies
            if (m_verticalScroll)
            {
                // Check if the thumb is being dragged
                if (m_mouseDownOnThumb)
                {
                    // Set the new value
                    if ((pos.y - m_mouseDownOnThumbPos.y - m_arrowUp.height) > 0)
                    {
                        // Calculate the new value
                        const unsigned int value = static_cast<unsigned int>((((pos.y - m_mouseDownOnThumbPos.y - m_arrowUp.height)
                            / (getSize().y - m_arrowUp.height - m_arrowDown.height - m_thumb.height)) * (m_maximum - m_viewportSize)) + 0.5f);

                        // If the value isn't too high then change it
                        if (value <= (m_maximum - m_viewportSize))
                            setValue(value);
                        else
                            setValue(m_maximum - m_viewportSize);
                    }
                    else // The mouse was above the scrollbar
                        setValue(0);

                    // Set the thumb position for smooth scrolling
                    const float thumbTop = pos.y - m_mouseDownOnThumbPos.y;
                    if ((thumbTop - m_arrowUp.height > 0) && (thumbTop + m_thumb.height + m_arrowDown.height < getSize().y))
                        m_thumb.top = thumbTop;
                    else // Prevent the thumb from going outside the scrollbar
                        m_thumb.top = m_track.top + ((m_track.height - m_thumb.height) * m_value / (m_maximum - m_viewportSize));
                }
                else // The click occurred on the track
                {
                    // If the position is positive then calculate the correct value
                    if (pos.y >= m_arrowUp.height)
                    {
                        // Make sure that you did not click on the down arrow
                        if (pos.y < getSize().y - m_arrowUp.height)
                        {
                            // Calculate the exact position (a number between 0 and maximum), as if the top of the thumb will be where you clicked
                            const float scaleFactor = (m_maximum - m_viewportSize) / (getSize().y - m_arrowUp.height - m_arrowDown.height - m_thumb.height);
                            const float value = (pos.y - m_arrowUp.height) * scaleFactor;

                            // Check if you clicked above the thumb
                            if (value <= m_value)
                            {
                                // Try to place the thumb on 2/3 of the clicked position
                                const float subtractValue = (m_thumb.height / 3.0f) * scaleFactor;
                                if (value >= subtractValue)
                                    setValue(static_cast<unsigned int>(value - subtractValue + 0.5f));
                                else
                                    setValue(0);
                            }
                            else // The click occurred below the thumb
                            {
                                // Try to place the thumb on 2/3 of the clicked position
                                const float subtractValue = (m_thumb.height * 2.0f / 3.0f) * scaleFactor;
                                if (value <= (m_maximum - m_viewportSize + subtractValue))
                                    setValue(static_cast<unsigned int>(value - subtractValue + 0.5f));
                                else
                                    setValue(m_maximum - m_viewportSize);
                            }
                        }
                    }

                    m_mouseDownOnThumbPos.x = pos.x - m_thumb.left;
                    m_mouseDownOnThumbPos.y = pos.y - m_thumb.top;
                    m_mouseDownOnThumb = true;
                }
            }
            else // the scrollbar lies horizontal
            {
                // Check if the thumb is being dragged
                if (m_mouseDownOnThumb)
                {
                    // Set the new value
                    if (pos.x - m_mouseDownOnThumbPos.x - m_arrowUp.width > 0)
                    {
                        // Calculate the new value
                        const unsigned int value = static_cast<unsigned int>((((pos.x - m_mouseDownOnThumbPos.x - m_arrowUp.width)
                            / (getSize().x - m_arrowUp.width - m_arrowDown.width - m_thumb.width)) * (m_maximum - m_viewportSize)) + 0.5f);

                        // If the value isn't too high then change it
                        if (value <= (m_maximum - m_viewportSize))
                            setValue(value);
                        else
                            setValue(m_maximum - m_viewportSize);
                    }
                    else // The mouse was to the left of the thumb
                        setValue(0);

                    // Set the thumb position for smooth scrolling
                    const float thumbLeft = pos.x - m_mouseDownOnThumbPos.x;
                    if ((thumbLeft - m_arrowUp.width > 0) && (thumbLeft + m_thumb.width + m_arrowDown.width < getSize().x))
                        m_thumb.left = thumbLeft;
                    else // Prevent the thumb from going outside the scrollbar
                        m_thumb.left = m_track.left + ((m_track.width - m_thumb.width) * m_value / (m_maximum - m_viewportSize));
                }
                else // The click occurred on the track
                {
                    // If the position is positive then calculate the correct value
                    if (pos.x >= m_arrowUp.width)
                    {
                        // Make sure that you did not click on the down arrow
                        if (pos.x < getSize().x - m_arrowUp.width)
                        {
                            // Calculate the exact position (a number between 0 and maximum), as if the left of the thumb will be where you clicked
                            const float scaleFactor = (m_maximum - m_viewportSize) / (getSize().x - m_arrowUp.width - m_arrowDown.width - m_thumb.width);
                            const float value = (pos.x - m_arrowUp.width) * scaleFactor;

                            // Check if you clicked to the left of the thumb
                            if (value <= m_value)
                            {
                                const float subtractValue = (m_thumb.width / 3.0f) * scaleFactor;

                                // Try to place the thumb on 2/3 of the clicked position
                                if (value >= subtractValue)
                                    setValue(static_cast<unsigned int>(value - subtractValue + 0.5f));
                                else
                                    setValue(0);
                            }
                            else // The click occurred to the right of the thumb
                            {
                                const float subtractValue = (m_thumb.width * 2.0f / 3.0f) * scaleFactor;

                                // Try to place the thumb on 2/3 of the clicked position
                                if (value <= (m_maximum - m_viewportSize + subtractValue))
                                    setValue(static_cast<unsigned int>(value - subtractValue + 0.5f));
                                else
                                    setValue(m_maximum - m_viewportSize);
                            }
                        }
                    }

                    m_mouseDownOnThumbPos.x = pos.x - m_thumb.left;
                    m_mouseDownOnThumbPos.y = pos.y - m_thumb.top;
                    m_mouseDownOnThumb = true;
                }
            }
        }

        if (FloatRect{m_thumb.left, m_thumb.top, m_thumb.width, m_thumb.height}.contains(pos))
            m_mouseHoverOverPart = Part::Thumb;
        else if (FloatRect{m_track.left, m_track.top, m_track.width, m_track.height}.contains(pos))
            m_mouseHoverOverPart = Part::Track;
        else if (FloatRect{m_arrowUp.left, m_arrowUp.top, m_arrowUp.width, m_arrowUp.height}.contains(pos))
            m_mouseHoverOverPart = Part::ArrowUp;
        else if (FloatRect{m_arrowDown.left, m_arrowDown.top, m_arrowDown.width, m_arrowDown.height}.contains(pos))
            m_mouseHoverOverPart = Part::ArrowDown;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    bool Scrollbar::mouseWheelScrolled(float delta, Vector2f pos)
    {
        if (static_cast<int>(m_value) - static_cast<int>(delta * m_scrollAmount) < 0)
            setValue(0);
        else
            setValue(static_cast<unsigned int>(m_value - (delta * m_scrollAmount)));

        // Update over which part the mouse is hovering
        mouseMoved(pos - getPosition());
        return true;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void Scrollbar::mouseNoLongerDown()
    {
        // The thumb might have been dragged between two values
        if (m_mouseDown)
            updateThumbPosition();

        Widget::mouseNoLongerDown();
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void Scrollbar::updateSize()
    {
        if (getSize().x <= getSize().y)
            m_verticalScroll = true;
        else
            m_verticalScroll = false;

        bool textured = false;
        if (m_spriteTrack.isSet() && m_spriteThumb.isSet() && m_spriteArrowUp.isSet() && m_spriteArrowDown.isSet())
            textured = true;

        if (m_verticalScroll)
        {
            m_arrowUp.width = getSize().x;
            m_arrowDown.width = getSize().x;

            if (textured)
            {
                m_arrowUp.height = getSize().x * m_spriteArrowUp.getTexture().getImageSize().x / m_spriteArrowUp.getTexture().getImageSize().y;
                m_arrowDown.height = getSize().x * m_spriteArrowDown.getTexture().getImageSize().x / m_spriteArrowDown.getTexture().getImageSize().y;
            }
            else
            {
                m_arrowUp.height = m_arrowUp.width;
                m_arrowDown.height = m_arrowUp.width;
            }

            m_track.width = getSize().x;
            m_track.height = std::max(0.f, getSize().y - m_arrowUp.height - m_arrowDown.height);

            m_thumb.width = getSize().x;
            if (m_maximum > m_viewportSize)
            {
                m_thumb.height = m_track.height * m_viewportSize / m_maximum;

                // Don't allow the thumb to become smaller than the scrollbar width, unless there isn't enough room for it
                if (m_thumb.height < getSize().x)
                {
                    if (getSize().x < m_track.height)
                        m_thumb.height = getSize().x;
                    else // Track is too small to contain minimum the thumb size, so instead we use 3/4th of track as size
                        m_thumb.height = m_track.height * 0.75f;
                }
            }
            else
                m_thumb.height = m_track.height;
        }
        else // The scrollbar lies horizontally
        {
            m_arrowUp.height = getSize().y;
            m_arrowDown.height = getSize().y;

            if (textured)
            {
                m_arrowUp.width = getSize().y * m_spriteArrowUp.getTexture().getImageSize().x / m_spriteArrowUp.getTexture().getImageSize().y;
                m_arrowDown.width = getSize().y * m_spriteArrowDown.getTexture().getImageSize().x / m_spriteArrowDown.getTexture().getImageSize().y;
            }
            else
            {
                m_arrowUp.width = m_arrowUp.height;
                m_arrowDown.width = m_arrowUp.height;
            }

            m_track.width = std::max(0.f, getSize().x - m_arrowUp.height - m_arrowDown.height);
            m_track.height = getSize().y;

            m_thumb.height = getSize().y;
            if (m_maximum > m_viewportSize)
            {
                m_thumb.width = m_track.width * m_viewportSize / m_maximum;

                // Don't allow the thumb to become smaller than the scrollbar width, unless there isn't enough room for it
                if (m_thumb.width < getSize().y)
                {
                    if (getSize().y < m_track.width)
                        m_thumb.width = getSize().y;
                    else // Track is too small to contain minimum the thumb size, so instead we use 3/4th of track as size
                        m_thumb.width = m_track.width * 0.75f;
                }
            }
            else
                m_thumb.width = m_track.width;
        }

        if (textured)
        {
            m_spriteArrowUp.setSize({m_arrowUp.width, m_arrowUp.height});
            m_spriteArrowUpHover.setSize({m_arrowUp.width, m_arrowUp.height});
            m_spriteArrowDown.setSize({m_arrowDown.width, m_arrowDown.height});
            m_spriteArrowDownHover.setSize({m_arrowDown.width, m_arrowDown.height});

            if (m_verticalScroll == m_verticalImage)
            {
                m_spriteTrack.setSize({m_track.width, m_track.height});
                m_spriteTrackHover.setSize({m_track.width, m_track.height});
                m_spriteThumb.setSize({m_thumb.width, m_thumb.height});
                m_spriteThumbHover.setSize({m_thumb.width, m_thumb.height});

                m_spriteTrack.setRotation(0);
                m_spriteTrackHover.setRotation(0);
                m_spriteThumb.setRotation(0);
                m_spriteThumbHover.setRotation(0);
            }
            else
            {
                m_spriteTrack.setSize({m_track.height, m_track.width});
                m_spriteTrackHover.setSize({m_track.height, m_track.width});
                m_spriteThumb.setSize({m_thumb.height, m_thumb.width});
                m_spriteThumbHover.setSize({m_thumb.height, m_thumb.width});

                m_spriteTrack.setRotation(-90);
                m_spriteTrackHover.setRotation(-90);
                m_spriteThumb.setRotation(-90);
                m_spriteThumbHover.setRotation(-90);
            }

            // Set the rotation or the arrows now that the size has been set
            if (m_verticalScroll)
            {
                m_spriteArrowUp.setRotation(0);
                m_spriteArrowUpHover.setRotation(0);
                m_spriteArrowDown.setRotation(0);
                m_spriteArrowDownHover.setRotation(0);
            }
            else
            {
                m_spriteArrowUp.setRotation(-90);
                m_spriteArrowUpHover.setRotation(-90);
                m_spriteArrowDown.setRotation(-90);
                m_spriteArrowDownHover.setRotation(-90);
            }
        }

        // Recalculate the position of the track, thumb and arrows
        if (m_verticalScroll)
        {
            m_arrowDown.left = 0;
            m_arrowDown.top = getSize().y - m_arrowDown.height;

            m_track.left = 0;
            m_track.top = m_arrowUp.height;
        }
        else
        {
            m_arrowDown.left = getSize().x - m_arrowDown.width;
            m_arrowDown.top = 0;

            m_track.left = m_arrowUp.width;
            m_track.top = 0;
        }

        updateThumbPosition();
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    Signal& Scrollbar::getSignal(std::string signalName)
    {
        if (signalName == toLower(onValueChange.getName()))
            return onValueChange;
        else
            return Widget::getSignal(std::move(signalName));
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void Scrollbar::rendererChanged(const std::string& property)
    {
        if (property == "texturetrack")
        {
            m_spriteTrack.setTexture(getSharedRenderer()->getTextureTrack());
            if (m_sizeSet)
                updateSize();
            else
            {
                if (m_verticalScroll)
                    setSize({getDefaultWidth(), getSize().y});
                else
                    setSize({getSize().x, getDefaultWidth()});

                m_sizeSet = false;
            }
        }
        else if (property == "texturetrackhover")
        {
            m_spriteTrackHover.setTexture(getSharedRenderer()->getTextureTrackHover());
        }
        else if (property == "texturethumb")
        {
            m_spriteThumb.setTexture(getSharedRenderer()->getTextureThumb());
            updateSize();
        }
        else if (property == "texturethumbhover")
        {
            m_spriteThumbHover.setTexture(getSharedRenderer()->getTextureThumbHover());
        }
        else if (property == "texturearrowup")
        {
            m_spriteArrowUp.setTexture(getSharedRenderer()->getTextureArrowUp());
            updateSize();
        }
        else if (property == "texturearrowuphover")
        {
            m_spriteArrowUpHover.setTexture(getSharedRenderer()->getTextureArrowUpHover());
        }
        else if (property == "texturearrowdown")
        {
            m_spriteArrowDown.setTexture(getSharedRenderer()->getTextureArrowDown());
            updateSize();
        }
        else if (property == "texturearrowdownhover")
        {
            m_spriteArrowDownHover.setTexture(getSharedRenderer()->getTextureArrowDownHover());
        }
        else if (property == "trackcolor")
        {
            m_trackColorCached = getSharedRenderer()->getTrackColor();
        }
        else if (property == "trackcolorhover")
        {
            m_trackColorHoverCached = getSharedRenderer()->getTrackColorHover();
        }
        else if (property == "thumbcolor")
        {
            m_thumbColorCached = getSharedRenderer()->getThumbColor();
        }
        else if (property == "thumbcolorhover")
        {
            m_thumbColorHoverCached = getSharedRenderer()->getThumbColorHover();
        }
        else if (property == "arrowbackgroundcolor")
        {
            m_arrowBackgroundColorCached = getSharedRenderer()->getArrowBackgroundColor();
        }
        else if (property == "arrowbackgroundcolorhover")
        {
            m_arrowBackgroundColorHoverCached = getSharedRenderer()->getArrowBackgroundColorHover();
        }
        else if (property == "arrowcolor")
        {
            m_arrowColorCached = getSharedRenderer()->getArrowColor();
        }
        else if (property == "arrowcolorhover")
        {
            m_arrowColorHoverCached = getSharedRenderer()->getArrowColorHover();
        }
        else if (property == "opacity")
        {
            Widget::rendererChanged(property);

            m_spriteTrack.setOpacity(m_opacityCached);
            m_spriteTrackHover.setOpacity(m_opacityCached);
            m_spriteThumb.setOpacity(m_opacityCached);
            m_spriteThumbHover.setOpacity(m_opacityCached);
            m_spriteArrowUp.setOpacity(m_opacityCached);
            m_spriteArrowUpHover.setOpacity(m_opacityCached);
            m_spriteArrowDown.setOpacity(m_opacityCached);
            m_spriteArrowDownHover.setOpacity(m_opacityCached);
        }
        else
            Widget::rendererChanged(property);
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    std::unique_ptr<DataIO::Node> Scrollbar::save(SavingRenderersMap& renderers) const
    {
        auto node = Widget::save(renderers);

        node->propertyValuePairs["AutoHide"] = std::make_unique<DataIO::ValueNode>(Serializer::serialize(m_autoHide));
        node->propertyValuePairs["ViewportSize"] = std::make_unique<DataIO::ValueNode>(to_string(m_viewportSize));
        node->propertyValuePairs["Maximum"] = std::make_unique<DataIO::ValueNode>(to_string(m_maximum));
        node->propertyValuePairs["Value"] = std::make_unique<DataIO::ValueNode>(to_string(m_value));
        node->propertyValuePairs["ScrollAmount"] = std::make_unique<DataIO::ValueNode>(to_string(m_scrollAmount));

        return node;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void Scrollbar::load(const std::unique_ptr<DataIO::Node>& node, const LoadingRenderersMap& renderers)
    {
        Widget::load(node, renderers);

        if (node->propertyValuePairs["viewportsize"])
            setViewportSize(tgui::stoi(node->propertyValuePairs["viewportsize"]->value));
        if (node->propertyValuePairs["maximum"])
            setMaximum(tgui::stoi(node->propertyValuePairs["maximum"]->value));
        if (node->propertyValuePairs["value"])
            setValue(tgui::stoi(node->propertyValuePairs["value"]->value));
        if (node->propertyValuePairs["scrollamount"])
            setScrollAmount(tgui::stoi(node->propertyValuePairs["scrollamount"]->value));
        if (node->propertyValuePairs["autohide"])
            setAutoHide(Deserializer::deserialize(ObjectConverter::Type::Bool, node->propertyValuePairs["autohide"]->value).getBool());
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void Scrollbar::updateThumbPosition()
    {
        if (m_verticalScroll)
        {
            m_thumb.left = 0;
            m_thumb.top = m_track.top + ((m_track.height - m_thumb.height) * m_value / (m_maximum - m_viewportSize));
        }
        else
        {
            m_thumb.left = m_track.left + ((m_track.width - m_thumb.width) * m_value / (m_maximum - m_viewportSize));
            m_thumb.top = 0;
        }
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void Scrollbar::draw(sf::RenderTarget& target, sf::RenderStates states) const
    {
        // Don't draw the scrollbar when it is not needed
        if (m_autoHide && (m_maximum <= m_viewportSize))
            return;

        states.transform.translate(getPosition());

        bool textured = false;
        if (m_spriteTrack.isSet() && m_spriteThumb.isSet() && m_spriteArrowUp.isSet() && m_spriteArrowDown.isSet())
            textured = true;

        // Draw arrow up/left
        if (textured)
        {
            if (m_mouseHover && m_spriteArrowUpHover.isSet() && (m_mouseHoverOverPart == Scrollbar::Part::ArrowUp))
                m_spriteArrowUpHover.draw(target, states);
            else
                m_spriteArrowUp.draw(target, states);
        }
        else
        {
            if (m_mouseHover && (m_mouseHoverOverPart == Scrollbar::Part::ArrowUp) && m_arrowBackgroundColorHoverCached.isSet())
                drawRectangleShape(target, states, {m_arrowUp.width, m_arrowUp.height}, m_arrowBackgroundColorHoverCached);
            else
                drawRectangleShape(target, states, {m_arrowUp.width, m_arrowUp.height}, m_arrowBackgroundColorCached);

            sf::ConvexShape arrow{3};
            if (m_mouseHover && (m_mouseHoverOverPart == Scrollbar::Part::ArrowUp) && m_arrowColorHoverCached.isSet())
                arrow.setFillColor(Color::calcColorOpacity(m_arrowColorHoverCached, m_opacityCached));
            else
                arrow.setFillColor(Color::calcColorOpacity(m_arrowColorCached, m_opacityCached));

            if (m_verticalScroll)
            {
                arrow.setPoint(0, {m_arrowUp.width / 5, m_arrowUp.height * 4/5});
                arrow.setPoint(1, {m_arrowUp.width / 2, m_arrowUp.height / 5});
                arrow.setPoint(2, {m_arrowUp.width * 4/5, m_arrowUp.height * 4/5});
            }
            else
            {
                arrow.setPoint(0, {m_arrowUp.width * 4/5, m_arrowUp.height / 5});
                arrow.setPoint(1, {m_arrowUp.width / 5, m_arrowUp.height / 2});
                arrow.setPoint(2, {m_arrowUp.width * 4/5, m_arrowUp.height * 4/5});
            }

            target.draw(arrow, states);
        }

        // Draw the track
        states.transform.translate(m_track.getPosition());
        if (textured)
        {
            if (m_mouseHover && m_spriteTrackHover.isSet() && (m_mouseHoverOverPart == Scrollbar::Part::Track))
                m_spriteTrackHover.draw(target, states);
            else
                m_spriteTrack.draw(target, states);
        }
        else
        {
            if (m_mouseHover && (m_mouseHoverOverPart == Scrollbar::Part::Track) && m_trackColorHoverCached.isSet())
                drawRectangleShape(target, states, {m_track.width, m_track.height}, m_trackColorHoverCached);
            else
                drawRectangleShape(target, states, {m_track.width, m_track.height}, m_trackColorCached);
        }
        states.transform.translate(-m_track.getPosition());

        // Draw the thumb
        states.transform.translate(m_thumb.getPosition());
        if (textured)
        {
            if (m_mouseHover && m_spriteThumbHover.isSet() && (m_mouseHoverOverPart == Scrollbar::Part::Thumb))
                m_spriteThumbHover.draw(target, states);
            else
                m_spriteThumb.draw(target, states);
        }
        else
        {
            if (m_mouseHover && (m_mouseHoverOverPart == Scrollbar::Part::Thumb) && m_thumbColorHoverCached.isSet())
                drawRectangleShape(target, states, {m_thumb.width, m_thumb.height}, m_thumbColorHoverCached);
            else
                drawRectangleShape(target, states, {m_thumb.width, m_thumb.height}, m_thumbColorCached);
        }
        states.transform.translate(-m_thumb.getPosition());

        // Draw arrow down/right
        states.transform.translate(m_arrowDown.getPosition());
        if (textured)
        {
            if (m_mouseHover && m_spriteArrowDownHover.isSet() && (m_mouseHoverOverPart == Scrollbar::Part::ArrowDown))
                m_spriteArrowDownHover.draw(target, states);
            else
                m_spriteArrowDown.draw(target, states);
        }
        else
        {
            if (m_mouseHover && (m_mouseHoverOverPart == Scrollbar::Part::ArrowDown) && m_arrowBackgroundColorHoverCached.isSet())
                drawRectangleShape(target, states, {m_arrowDown.width, m_arrowDown.height}, m_arrowBackgroundColorHoverCached);
            else
                drawRectangleShape(target, states, {m_arrowDown.width, m_arrowDown.height}, m_arrowBackgroundColorCached);

            sf::ConvexShape arrow{3};
            if (m_mouseHover && (m_mouseHoverOverPart == Scrollbar::Part::ArrowDown) && m_arrowColorHoverCached.isSet())
                arrow.setFillColor(Color::calcColorOpacity(m_arrowColorHoverCached, m_opacityCached));
            else
                arrow.setFillColor(Color::calcColorOpacity(m_arrowColorCached, m_opacityCached));

            if (m_verticalScroll)
            {
                arrow.setPoint(0, {m_arrowDown.width / 5, m_arrowDown.height / 5});
                arrow.setPoint(1, {m_arrowDown.width / 2, m_arrowDown.height * 4/5});
                arrow.setPoint(2, {m_arrowDown.width * 4/5, m_arrowDown.height / 5});
            }
            else // Spin button lies horizontal
            {
                arrow.setPoint(0, {m_arrowDown.width / 5, m_arrowDown.height / 5});
                arrow.setPoint(1, {m_arrowDown.width * 4/5, m_arrowDown.height / 2});
                arrow.setPoint(2, {m_arrowDown.width / 5, m_arrowDown.height * 4/5});
            }

            target.draw(arrow, states);
        }
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    bool ScrollbarChildWidget::isMouseDown() const
    {
        return m_mouseDown;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    bool ScrollbarChildWidget::isMouseDownOnThumb() const
    {
        return m_mouseDownOnThumb;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    bool ScrollbarChildWidget::isShown() const
    {
        return m_visible && (!m_autoHide || (m_maximum > m_viewportSize));
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// TGUI - Texus' Graphical User Interface
// Copyright (C) 2012-2018 Bruno Van de Velde (vdv_b@tgui.eu)
//
// This software is provided 'as-is', without any express or implied warranty.
// In no event will the authors be held liable for any damages arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it freely,
// subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented;
//    you must not claim that you wrote the original software.
//    If you use this software in a product, an acknowledgment
//    in the product documentation would be appreciated but is not required.
//
// 2. Altered source versions must be plainly marked as such,
//    and must not be misrepresented as being the original software.
//
// 3. This notice may not be removed or altered from any source distribution.
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


#include <TGUI/Widgets/Tabs.hpp>
#include <TGUI/Clipping.hpp>

#ifdef TGUI_USE_CPP17
    #include <optional>
#endif

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace tgui
{
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    Tabs::Tabs()
    {
        m_type = "Tabs";
        m_distanceToSideCached = Text::getLineHeight(m_fontCached, getGlobalTextSize()) * 0.4f;

        m_renderer = aurora::makeCopied<TabsRenderer>();
        setRenderer(Theme::getDefault()->getRendererNoThrow(m_type));

        setTextSize(getGlobalTextSize());
        setTabHeight(Text::getLineHeight(m_fontCached, m_textSize) * 1.25f + m_bordersCached.getTop() + m_bordersCached.getBottom());
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    Tabs::Ptr Tabs::create()
    {
        return std::make_shared<Tabs>();
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    Tabs::Ptr Tabs::copy(Tabs::ConstPtr tabs)
    {
        if (tabs)
            return std::static_pointer_cast<Tabs>(tabs->clone());
        else
            return nullptr;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    TabsRenderer* Tabs::getSharedRenderer()
    {
        return aurora::downcast<TabsRenderer*>(Widget::getSharedRenderer());
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    const TabsRenderer* Tabs::getSharedRenderer() const
    {
        return aurora::downcast<const TabsRenderer*>(Widget::getSharedRenderer());
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    TabsRenderer* Tabs::getRenderer()
    {
        return aurora::downcast<TabsRenderer*>(Widget::getRenderer());
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    const TabsRenderer* Tabs::getRenderer() const
    {
        return aurora::downcast<const TabsRenderer*>(Widget::getRenderer());
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void Tabs::setSize(const Layout2d& size)
    {
        Widget::setSize(size);

        // Tabs is no longer auto-sizing
        m_autoSize = false;
        recalculateTabsWidth();
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void Tabs::setEnabled(bool enabled)
    {
        Widget::setEnabled(enabled);

        if (!enabled)
            deselect();

        updateTextColors();
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void Tabs::setAutoSize(bool autoSize)
    {
        if (m_autoSize != autoSize)
        {
            m_autoSize = autoSize;
            recalculateTabsWidth();
        }
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    bool Tabs::getAutoSize() const
    {
        return m_autoSize;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    std::size_t Tabs::add(const sf::String& text, bool selectTab)
    {
        // Use the insert function to put the tab in the right place
        insert(m_tabs.size(), text, selectTab);

        // Return the index of the new tab
        return m_tabs.size()-1;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void Tabs::insert(std::size_t index, const sf::String& text, bool selectTab)
    {
        // If the index is too high then just insert at the end
        if (index > m_tabs.size())
            index = m_tabs.size();

        // Create the new tab
        Tab newTab;
        newTab.visible = true;
        newTab.enabled = true;
        newTab.width = 0;
        newTab.text.setFont(m_fontCached);
        newTab.text.setColor(m_textColorCached);
        newTab.text.setOpacity(m_opacityCached);
        newTab.text.setCharacterSize(getTextSize());
        newTab.text.setString(text);

        m_tabs.insert(m_tabs.begin() + index, std::move(newTab));
        recalculateTabsWidth();

        // New hovered tab depends on several factors, we keep it simple and just remove the hover state
        m_hoveringTab = -1;

        if (m_selectedTab >= static_cast<int>(index))
            m_selectedTab++;

        // If the tab has to be selected then do so
        if (selectTab)
            select(index);
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    sf::String Tabs::getText(std::size_t index) const
    {
        if (index >= m_tabs.size())
            return "";
        else
            return m_tabs[index].text.getString();
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    bool Tabs::changeText(std::size_t index, const sf::String& text)
    {
        if (index >= m_tabs.size())
            return false;

        m_tabs[index].text.setString(text);
        recalculateTabsWidth();
        return true;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    bool Tabs::select(const sf::String& text)
    {
        for (unsigned int i = 0; i < m_tabs.size(); ++i)
        {
            if (m_tabs[i].text.getString() == text)
                return select(i);
        }

        deselect();
        return false;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    bool Tabs::select(std::size_t index)
    {
        // Don't select a tab that is already selected
        if (m_selectedTab == static_cast<int>(index))
            return true;

        // If the index is too high or if the tab is invisible or disabled then we can't select it
        if ((index >= m_tabs.size()) || !m_enabled || !m_tabs[index].visible || !m_tabs[index].enabled)
        {
            deselect();
            return false;
        }

        if (m_selectedTab >= 0)
            m_tabs[m_selectedTab].text.setColor(m_textColorCached);

        // Select the tab
        m_selectedTab = static_cast<int>(index);
        m_tabs[m_selectedTab].text.setColor(m_selectedTextColorCached);

        // Send the callback
        onTabSelect.emit(this, m_tabs[index].text.getString());
        return true;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void Tabs::deselect()
    {
        if (m_selectedTab >= 0)
        {
            m_tabs[m_selectedTab].text.setColor(m_textColorCached);
            m_selectedTab = -1;
        }
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    bool Tabs::remove(const sf::String& text)
    {
        for (unsigned int i = 0; i < m_tabs.size(); ++i)
        {
            if (m_tabs[i].text.getString() == text)
                return remove(i);
        }

        return false;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    bool Tabs::remove(std::size_t index)
    {
        // The index can't be too high
        if (index > m_tabs.size() - 1)
            return false;

        // Remove the tab
        m_tabs.erase(m_tabs.begin() + index);

        // Check if the selected tab should be updated
        if (m_selectedTab == static_cast<int>(index))
            m_selectedTab = -1;
        else if (m_selectedTab > static_cast<int>(index))
            --m_selectedTab;

        // New hovered tab depends on several factors, we keep it simple and just remove the hover state
        m_hoveringTab = -1;
        return true;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void Tabs::removeAll()
    {
        m_tabs.clear();
        m_selectedTab = -1;
        m_hoveringTab = -1;

        recalculateTabsWidth();
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    sf::String Tabs::getSelected() const
    {
        if (m_selectedTab >= 0)
            return m_tabs[m_selectedTab].text.getString();
        else
            return "";
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    int Tabs::getSelectedIndex() const
    {
        return m_selectedTab;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void Tabs::setTabVisible(std::size_t index, bool visible)
    {
        if (index >= m_tabs.size())
            return;

        m_tabs[index].visible = visible;
        recalculateTabsWidth();
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    bool Tabs::getTabVisible(std::size_t index) const
    {
        if (index >= m_tabs.size())
            return false;

        return m_tabs[index].visible;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void Tabs::setTabEnabled(std::size_t index, bool enabled)
    {
        if (index >= m_tabs.size())
            return;

        m_tabs[index].enabled = enabled;
        updateTextColors();
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    bool Tabs::getTabEnabled(std::size_t index) const
    {
        if (index >= m_tabs.size())
            return false;

        return m_tabs[index].enabled;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void Tabs::setTextSize(unsigned int size)
    {
        if ((size == 0) || (m_requestedTextSize != size))
        {
            m_requestedTextSize = size;

            if (size == 0)
                m_textSize = Text::findBestTextSize(m_fontCached, (getSize().y - m_bordersCached.getTop() - m_bordersCached.getBottom()) * 0.8f);
            else
                m_textSize = size;

            for (auto& tab : m_tabs)
                tab.text.setCharacterSize(m_textSize);

            recalculateTabsWidth();
        }
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    unsigned int Tabs::getTextSize() const
    {
        return m_textSize;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void Tabs::setTabHeight(float height)
    {
        Widget::setSize({getSizeLayout().x, height});
        m_bordersCached.updateParentSize(getSize());

        // Recalculate the size when the text is auto sizing
        if (m_requestedTextSize == 0)
            setTextSize(0);
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void Tabs::setMaximumTabWidth(float maximumWidth)
    {
        m_maximumTabWidth = maximumWidth;

        recalculateTabsWidth();
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    float Tabs::getMaximumTabWidth() const
    {
        return m_maximumTabWidth;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void Tabs::setMinimumTabWidth(float minimumWidth)
    {
        m_minimumTabWidth = minimumWidth;

        recalculateTabsWidth();
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    float Tabs::getMinimumTabWidth() const
    {
        return m_minimumTabWidth;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    std::size_t Tabs::getTabsCount() const
    {
        return m_tabs.size();
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    bool Tabs::mouseOnWidget(Vector2f pos) const
    {
        return FloatRect{getPosition().x, getPosition().y, getSize().x, getSize().y}.contains(pos);
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void Tabs::leftMousePressed(Vector2f pos)
    {
        pos -= getPosition();

        float width = m_bordersCached.getLeft() / 2.f;
        for (unsigned int i = 0; i < m_tabs.size(); ++i)
        {
            if (!m_tabs[i].visible)
                continue;

            // Append the width of the tab
            width += (m_bordersCached.getLeft() / 2.f) + m_tabs[i].width + (m_bordersCached.getRight() / 2.0f);

            // If the mouse went down on this tab then select it
            if (pos.x < width)
            {
                select(i);
                break;
            }
        }
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void Tabs::mouseMoved(Vector2f pos)
    {
        Widget::mouseMoved(pos);

        pos -= getPosition();
        m_hoveringTab = -1;
        float width = m_bordersCached.getLeft() / 2.f;

        for (unsigned int i = 0; i < m_tabs.size(); ++i)
        {
            if (!m_tabs[i].visible)
                continue;

            // Append the width of the tab
            width += (m_bordersCached.getLeft() / 2.f) + m_tabs[i].width + (m_bordersCached.getRight() / 2.0f);

            // If the mouse is on top of this tab then remember it
            if (pos.x < width)
            {
                if (m_tabs[i].enabled)
                    m_hoveringTab = i;

                break;
            }
        }
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void Tabs::mouseNoLongerOnWidget()
    {
        Widget::mouseNoLongerOnWidget();

        m_hoveringTab = -1;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void Tabs::recalculateTabsWidth()
    {
        unsigned int visibleTabs = 0;
        for (unsigned int i = 0; i < m_tabs.size(); ++i)
        {
            if (m_tabs[i].visible)
                visibleTabs++;
        }

        if (m_autoSize)
        {
            if (m_tabs.empty())
                Widget::setSize({m_bordersCached.getLeft() + m_bordersCached.getRight(), getSizeLayout().y});
            else
            {
                // First calculate the width of the tabs as if there aren't any borders
                float totalWidth = 0;
                for (unsigned int i = 0; i < m_tabs.size(); ++i)
                {
                    if (!m_tabs[i].visible)
                        continue;

                    m_tabs[i].width = m_tabs[i].text.getSize().x + std::max(m_minimumTabWidth, 2 * m_distanceToSideCached);
                    if ((m_maximumTabWidth > 0) && (m_maximumTabWidth < m_tabs[i].width))
                        m_tabs[i].width = m_maximumTabWidth;

                    totalWidth += m_tabs[i].width;
                }

                // Now add the borders to the tabs
                totalWidth += (visibleTabs + 1) * ((m_bordersCached.getLeft() + m_bordersCached.getRight()) / 2.f);

                Widget::setSize({totalWidth, getSizeLayout().y});
            }
        }
        else // A size was provided
        {
            const float tabWidth = (getSize().x - ((visibleTabs + 1) * ((m_bordersCached.getLeft() + m_bordersCached.getRight()) / 2.f))) / visibleTabs;
            for (unsigned int i = 0; i < m_tabs.size(); ++i)
                m_tabs[i].width = tabWidth;
        }

        m_bordersCached.updateParentSize(getSize());
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    Signal& Tabs::getSignal(std::string signalName)
    {
        if (signalName == toLower(onTabSelect.getName()))
            return onTabSelect;
        else
            return Widget::getSignal(std::move(signalName));
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void Tabs::rendererChanged(const std::string& property)
    {
        if (property == "borders")
        {
            m_bordersCached = getSharedRenderer()->getBorders();
            recalculateTabsWidth();
        }
        else if (property == "textcolor")
        {
            m_textColorCached = getSharedRenderer()->getTextColor();
            updateTextColors();
        }
        else if (property == "textcolorhover")
        {
            m_textColorHoverCached = getSharedRenderer()->getTextColorHover();
            updateTextColors();
        }
        else if (property == "textcolordisabled")
        {
            m_textColorDisabledCached = getSharedRenderer()->getTextColorDisabled();
            updateTextColors();
        }
        else if (property == "selectedtextcolor")
        {
            m_selectedTextColorCached = getSharedRenderer()->getSelectedTextColor();
            updateTextColors();
        }
        else if (property == "selectedtextcolorhover")
        {
            m_selectedTextColorHoverCached = getSharedRenderer()->getSelectedTextColorHover();
            updateTextColors();
        }
        else if (property == "texturetab")
        {
            m_spriteTab.setTexture(getSharedRenderer()->getTextureTab());
        }
        else if (property == "texturetabhover")
        {
            m_spriteTabHover.setTexture(getSharedRenderer()->getTextureTabHover());
        }
        else if (property == "textureselectedtab")
        {
            m_spriteSelectedTab.setTexture(getSharedRenderer()->getTextureSelectedTab());
        }
        else if (property == "textureselectedtabhover")
        {
            m_spriteSelectedTabHover.setTexture(getSharedRenderer()->getTextureSelectedTabHover());
        }
        else if (property == "texturedisabledtab")
        {
            m_spriteDisabledTab.setTexture(getSharedRenderer()->getTextureDisabledTab());
        }
        else if (property == "distancetoside")
        {
            m_distanceToSideCached = getSharedRenderer()->getDistanceToSide();
            recalculateTabsWidth();
        }
        else if (property == "backgroundcolor")
        {
            m_backgroundColorCached = getSharedRenderer()->getBackgroundColor();
        }
        else if (property == "backgroundcolorhover")
        {
            m_backgroundColorHoverCached = getSharedRenderer()->getBackgroundColorHover();
        }
        else if (property == "backgroundcolordisabled")
        {
            m_backgroundColorDisabledCached = getSharedRenderer()->getBackgroundColorDisabled();
        }
        else if (property == "selectedbackgroundcolor")
        {
            m_selectedBackgroundColorCached = getSharedRenderer()->getSelectedBackgroundColor();
        }
        else if (property == "selectedbackgroundcolorhover")
        {
            m_selectedBackgroundColorHoverCached = getSharedRenderer()->getSelectedBackgroundColorHover();
        }
        else if (property == "bordercolor")
        {
            m_borderColorCached = getSharedRenderer()->getBorderColor();
        }
        else if (property == "opacity")
        {
            Widget::rendererChanged(property);

            m_spriteTab.setOpacity(m_opacityCached);
            m_spriteTabHover.setOpacity(m_opacityCached);
            m_spriteSelectedTab.setOpacity(m_opacityCached);
            m_spriteSelectedTabHover.setOpacity(m_opacityCached);
            m_spriteDisabledTab.setOpacity(m_opacityCached);

            for (auto& tab : m_tabs)
                tab.text.setOpacity(m_opacityCached);
        }
        else if (property == "font")
        {
            Widget::rendererChanged(property);

            for (auto& tab : m_tabs)
                tab.text.setFont(m_fontCached);

            // Recalculate the size when the text is auto sizing
            if (m_requestedTextSize == 0)
                setTextSize(0);
            else
                recalculateTabsWidth();
        }
        else
            Widget::rendererChanged(property);
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    std::unique_ptr<DataIO::Node> Tabs::save(SavingRenderersMap& renderers) const
    {
        auto node = Widget::save(renderers);

        if (m_tabs.size() > 0)
        {
            bool allTabsVisible = true;
            bool allTabsEnabled = true;
            std::string tabList = "[" + Serializer::serialize(getText(0));
            std::string tabVisibleList = "[" + Serializer::serialize(getTabVisible(0));
            std::string tabEnabledList = "[" + Serializer::serialize(getTabEnabled(0));
            for (std::size_t i = 1; i < m_tabs.size(); ++i)
            {
                tabList += ", " + Serializer::serialize(getText(i));
                tabVisibleList += ", " + Serializer::serialize(getTabVisible(i));
                tabEnabledList += ", " + Serializer::serialize(getTabEnabled(i));

                if (!getTabVisible(i))
                    allTabsVisible = false;
                if (!getTabEnabled(i))
                    allTabsEnabled = false;
            }

            tabList += "]";
            tabVisibleList += "]";
            tabEnabledList += "]";

            node->propertyValuePairs["Tabs"] = std::make_unique<DataIO::ValueNode>(tabList);
            if (!allTabsVisible)
                node->propertyValuePairs["TabsVisible"] = std::make_unique<DataIO::ValueNode>(tabVisibleList);
            if (!allTabsEnabled)
                node->propertyValuePairs["TabsEnabled"] = std::make_unique<DataIO::ValueNode>(tabEnabledList);
        }

        if (getSelectedIndex() >= 0)
            node->propertyValuePairs["Selected"] = std::make_unique<DataIO::ValueNode>(to_string(getSelectedIndex()));

        if (m_maximumTabWidth > 0)
            node->propertyValuePairs["MaximumTabWidth"] = std::make_unique<DataIO::ValueNode>(to_string(m_maximumTabWidth));

        if (m_autoSize)
        {
            node->propertyValuePairs.erase("Size");
            node->propertyValuePairs["TabHeight"] = std::make_unique<DataIO::ValueNode>(to_string(getSize().y));
        }

        node->propertyValuePairs["TextSize"] = std::make_unique<DataIO::ValueNode>(to_string(m_textSize));
        node->propertyValuePairs["AutoSize"] = std::make_unique<DataIO::ValueNode>(to_string(m_autoSize));

        return node;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void Tabs::load(const std::unique_ptr<DataIO::Node>& node, const LoadingRenderersMap& renderers)
    {
        Widget::load(node, renderers);

        if (node->propertyValuePairs["tabs"])
        {
            if (!node->propertyValuePairs["tabs"]->listNode)
                throw Exception{"Failed to parse 'Tabs' property, expected a list as value"};

            for (const auto& tabText : node->propertyValuePairs["tabs"]->valueList)
                add(Deserializer::deserialize(ObjectConverter::Type::String, tabText).getString());
        }

        if (node->propertyValuePairs["tabsvisible"])
        {
            if (!node->propertyValuePairs["tabsvisible"]->listNode)
                throw Exception{"Failed to parse 'TabsVisible' property, expected a list as value"};

            const auto& values = node->propertyValuePairs["tabsvisible"]->valueList;
            for (unsigned int i = 0; i < values.size(); ++i)
                setTabVisible(i, Deserializer::deserialize(ObjectConverter::Type::Bool, values[i]).getBool());
        }

        if (node->propertyValuePairs["tabsenabled"])
        {
            if (!node->propertyValuePairs["tabsenabled"]->listNode)
                throw Exception{"Failed to parse 'TabsEnabled' property, expected a list as value"};

            const auto& values = node->propertyValuePairs["tabsenabled"]->valueList;
            for (unsigned int i = 0; i < values.size(); ++i)
                setTabEnabled(i, Deserializer::deserialize(ObjectConverter::Type::Bool, values[i]).getBool());
        }

        if (node->propertyValuePairs["maximumtabwidth"])
            setMaximumTabWidth(tgui::stof(node->propertyValuePairs["maximumtabwidth"]->value));
        if (node->propertyValuePairs["textsize"])
            setTextSize(tgui::stoi(node->propertyValuePairs["textsize"]->value));
        if (node->propertyValuePairs["tabheight"])
            setTabHeight(tgui::stof(node->propertyValuePairs["tabheight"]->value));
        if (node->propertyValuePairs["selected"])
            select(tgui::stoi(node->propertyValuePairs["selected"]->value));
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void Tabs::updateTextColors()
    {
        for (auto& tab : m_tabs)
        {
            if ((!m_enabled || !tab.enabled) && m_textColorDisabledCached.isSet())
                tab.text.setColor(m_textColorDisabledCached);
            else
                tab.text.setColor(m_textColorCached);
        }

        if (m_selectedTab >= 0)
        {
            if ((m_selectedTab == m_hoveringTab) && m_selectedTextColorHoverCached.isSet())
                m_tabs[m_selectedTab].text.setColor(m_selectedTextColorHoverCached);
            else if (m_selectedTextColorCached.isSet())
                m_tabs[m_selectedTab].text.setColor(m_selectedTextColorCached);
        }

        if ((m_hoveringTab >= 0) && (m_selectedTab != m_hoveringTab))
        {
            if (m_textColorHoverCached.isSet())
                m_tabs[m_hoveringTab].text.setColor(m_textColorHoverCached);
        }
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void Tabs::draw(sf::RenderTarget& target, sf::RenderStates states) const
    {
        states.transform.translate(getPosition());
        states.transform.translate({});

        // Draw the borders around the tabs
        if (m_bordersCached != Borders{0})
        {
            drawBorders(target, states, m_bordersCached, getSize(), m_borderColorCached);
            states.transform.translate({m_bordersCached.getLeft(), m_bordersCached.getTop()});
        }

        const float usableHeight = getSize().y - m_bordersCached.getTop() - m_bordersCached.getBottom();
        for (unsigned int i = 0; i < m_tabs.size(); ++i)
        {
            if (!m_tabs[i].visible)
                continue;

            sf::RenderStates textStates = states;

            // Draw the background of the tab
            if (m_spriteTab.isSet() && m_spriteSelectedTab.isSet())
            {
                Sprite spriteTab;
                if ((!m_enabled || !m_tabs[i].enabled) && m_spriteDisabledTab.isSet())
                    spriteTab = m_spriteDisabledTab;
                else if (m_selectedTab == static_cast<int>(i))
                {
                    if ((m_hoveringTab == static_cast<int>(i)) && m_spriteSelectedTabHover.isSet())
                        spriteTab = m_spriteSelectedTabHover;
                    else
                        spriteTab = m_spriteSelectedTab;
                }
                else if ((m_hoveringTab == static_cast<int>(i)) && m_spriteTabHover.isSet())
                    spriteTab = m_spriteTabHover;
                else
                    spriteTab = m_spriteTab;

                spriteTab.setSize({m_tabs[i].width, usableHeight});
                spriteTab.draw(target, states);
            }
            else // No texture was loaded
            {
                if ((!m_enabled || !m_tabs[i].enabled) && m_backgroundColorDisabledCached.isSet())
                    drawRectangleShape(target, states, {m_tabs[i].width, usableHeight}, m_backgroundColorDisabledCached);
                else if (m_selectedTab == static_cast<int>(i))
                {
                    if ((m_hoveringTab == static_cast<int>(i)) && m_selectedBackgroundColorHoverCached.isSet())
                        drawRectangleShape(target, states, {m_tabs[i].width, usableHeight}, m_selectedBackgroundColorHoverCached);
                    else
                        drawRectangleShape(target, states, {m_tabs[i].width, usableHeight}, m_selectedBackgroundColorCached);
                }
                else if ((m_hoveringTab == static_cast<int>(i)) && m_backgroundColorHoverCached.isSet())
                    drawRectangleShape(target, states, {m_tabs[i].width, usableHeight}, m_backgroundColorHoverCached);
                else
                    drawRectangleShape(target, states, {m_tabs[i].width, usableHeight}, m_backgroundColorCached);
            }

            // Draw the borders between the tabs
            states.transform.translate({m_tabs[i].width, 0});
            if ((m_bordersCached != Borders{0}) && (i < m_tabs.size() - 1))
            {
                drawRectangleShape(target, states, {(m_bordersCached.getLeft() + m_bordersCached.getRight()) / 2.f, usableHeight}, m_borderColorCached);
                states.transform.translate({(m_bordersCached.getLeft() + m_bordersCached.getRight()) / 2.f, 0});
            }

            // Apply clipping if required for the text in this tab
            const float usableWidth = m_tabs[i].width - (2 * m_distanceToSideCached);
        #ifdef TGUI_USE_CPP17
            std::optional<Clipping> clipping;
            if (m_tabs[i].text.getSize().x > usableWidth)
                clipping.emplace(target, textStates, Vector2f{m_distanceToSideCached, 0}, Vector2f{usableWidth, usableHeight});
        #else
            std::unique_ptr<Clipping> clipping;
            if (m_tabs[i].text.getSize().x > usableWidth)
                clipping = std::make_unique<Clipping>(target, textStates, Vector2f{m_distanceToSideCached, 0}, Vector2f{usableWidth, usableHeight});
        #endif

            // Draw the text
            textStates.transform.translate({m_distanceToSideCached + ((usableWidth - m_tabs[i].text.getSize().x) / 2.f), ((usableHeight - m_tabs[i].text.getSize().y) / 2.f)});
            m_tabs[i].text.draw(target, textStates);
        }
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// TGUI - Texus' Graphical User Interface
// Copyright (C) 2012-2018 Bruno Van de Velde (vdv_b@tgui.eu)
//
// This software is provided 'as-is', without any express or implied warranty.
// In no event will the authors be held liable for any damages arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it freely,
// subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented;
//    you must not claim that you wrote the original software.
//    If you use this software in a product, an acknowledgment
//    in the product documentation would be appreciated but is not required.
//
// 2. Altered source versions must be plainly marked as such,
//    and must not be misrepresented as being the original software.
//
// 3. This notice may not be removed or altered from any source distribution.
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


#include <TGUI/Widgets/VerticalLayout.hpp>
#include <numeric>

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace tgui
{
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    VerticalLayout::VerticalLayout(const Layout2d& size) :
        BoxLayoutRatios{size}
    {
        m_type = "VerticalLayout";

        m_renderer = aurora::makeCopied<BoxLayoutRenderer>();
        setRenderer(Theme::getDefault()->getRendererNoThrow(m_type));
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    VerticalLayout::Ptr VerticalLayout::create(const Layout2d& size)
    {
        return std::make_shared<VerticalLayout>(size);
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    VerticalLayout::Ptr VerticalLayout::copy(VerticalLayout::ConstPtr layout)
    {
        if (layout)
            return std::static_pointer_cast<VerticalLayout>(layout->clone());
        else
            return nullptr;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void VerticalLayout::updateWidgets()
    {
        const float totalSpaceBetweenWidgets = (m_spaceBetweenWidgetsCached * m_widgets.size()) - m_spaceBetweenWidgetsCached;
        const Vector2f contentSize = {getSize().x - m_paddingCached.getLeft() - m_paddingCached.getRight(),
                                          getSize().y - m_paddingCached.getTop() - m_paddingCached.getBottom()};

        const float totalRatio = std::accumulate(m_ratios.begin(), m_ratios.end(), 0.f);

        float currentOffset = 0;
        for (std::size_t i = 0; i < m_widgets.size(); ++i)
        {
            auto& widget = m_widgets[i];
            const float height = (contentSize.y - totalSpaceBetweenWidgets) * (m_ratios[i] / totalRatio);

            widget->setSize({contentSize.x, height});
            widget->setPosition({0, currentOffset});

            // Correct the size for widgets that are bigger than what you set (e.g. have borders around it or a text next to them)
            if (widget->getFullSize() != widget->getSize())
            {
                const Vector2f newSize = widget->getSize() - (widget->getFullSize() - widget->getSize());
                if (newSize.x > 0 && newSize.y > 0)
                {
                    widget->setSize(newSize);
                    widget->setPosition(widget->getPosition() - widget->getWidgetOffset());
                }
            }

            currentOffset += height + m_spaceBetweenWidgetsCached;
        }
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
