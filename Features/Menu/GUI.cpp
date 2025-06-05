#include "GUI.h"
#include "NccMath.h"
#include "NccDraw.h"
#include "NccUtilities.h"
#include "NccDraw.h"
#include <array>
#pragma warning(disable : 4244)
CObject::CObject(int x, int y, int width, int height, int layer)
	: m_x(x), m_y(y), m_width(width), m_height(height), m_layer(layer)
{}

//--------------------------------------------------------------------------------

void CObjectContainer::AddControl(CObject* obj)
{
	Objects.push_back(obj);
}

//--------------------------------------------------------------------------------

void CForm::Paint()
{
	for (auto& w : Windows)
	{
		w.Paint();
	}
}

void CForm::Click()
{
	for (auto& w : Windows)
	{
		w.Click();
	}
}

void CForm::HandleInput(WPARAM vk)
{
	for (auto& w : Windows)
	{
		w.HandleInput(vk);
	}
}

//--------------------------------------------------------------------------------

CWindow::CWindow(int x, int y, int width, int height, bool* open)
	: CObject(x, y, width, height), m_open(open)
{}


void CWindow::Paint(int t_x, int t_y, float* controlheight)
{

	if (m_open != nullptr && !*m_open) return;

	if (m_dragging && !InputSys::Get().IsKeyDown(VK_LBUTTON))
		m_dragging = false;

	int cur_x, cur_y;
	I::MatSystemSurface->SurfaceGetCursorPos(cur_x, cur_y);

	if (m_dragging && InputSys::Get().IsKeyDown(VK_LBUTTON) && InputSys::Get().GetKeyState(VK_LBUTTON) != KeyState::Pressed)
	{
		m_x = cur_x - m_dragoffsetx;
		m_y = cur_y - m_dragoffsety;
	}

	Vector2D tl(t_x + m_x, t_y + m_y);
	Vector2D br(tl.x + m_width, tl.y + m_height);

	if (InputSys::Get().GetKeyState(VK_LBUTTON) == KeyState::Pressed)
		if (Math::InBox(tl.x, tl.y - 16, br.x, tl.y, cur_x, cur_y))
		{
			m_dragging = true;
			m_dragoffsetx = cur_x - m_x;
			m_dragoffsety = cur_y - m_y;
		}

	Draw::FilledRectangle(tl.x, tl.y - 16, br.x, br.y, Color(248, 0, 233, 135));
	Draw::Rectangle(tl.x, tl.y - 16, br.x, br.y, Color(248, 5, 233, 255));
	Draw::Rectangle(tl.x + 1, tl.y - 15, br.x - 1, br.y - 1, Color(255, 5, 226, 255));
	tl += 6;
	br -= 6;
	Draw::FilledGradientRectangle(tl.x, tl.y, br.x, br.y, Color(238, 238, 238, 255), Color(210, 210, 210, 255));
	Draw::Rectangle(tl, br, Color(50, 5, 5, 255));
	Draw::Rectangle(tl.x - 1, tl.y - 1, br.x + 1, br.y + 1, Color(255, 5, 226, 255));

	Draw::Text(tl.x + 3, tl.y - 17, L"Astolfoware For L4D2", Font::Get().MenuText, Color(255, 255, 255, 255));

	m_controlheight = 0;
	for (auto o : Objects)
	{
		o->Paint(tl.x + 5, tl.y + 5, &m_controlheight);
	}
}

void CWindow::Click()
{
	if (m_open != nullptr && !*m_open) return;

	for (auto o : Objects)
	{
		o->Click();
	}
}

void CWindow::HandleInput(WPARAM vk)
{
	if (m_open != nullptr && !*m_open) return;

	for (auto o : Objects)
	{
		o->HandleInput(vk);
	}
}

//--------------------------------------------------------------------------------

CChild::CChild(int x, int y, int width, std::wstring title)
	: CObject(x, y, width), m_title(title)
{}

void CChild::Paint(int t_x, int t_y, float* controlheight)
{
	Vector2D tl(t_x + m_x, t_y + m_y + 3);
	Vector2D br(t_x + m_x + m_width, t_y + m_y + m_controlheight + 15);

	auto textSize = Draw::GetTextSize(m_title.data(), Font::Get().MenuText);

	// background
	Draw::FilledGradientRectangle(tl.x, tl.y, br.x, br.y, Color(210, 210, 210, 255), Color(238, 238, 238, 255));

	Color color = Color(180, 180, 180, 255);

	// top
	Draw::Line(tl.x, tl.y, tl.x + 8, tl.y, color);
	Draw::Line(tl.x + 18 + textSize.x, tl.y, br.x, tl.y, color);
	// right
	Draw::Line(br.x, tl.y, br.x, br.y, color);
	//bottom
	Draw::Line(tl.x, br.y, br.x, br.y, color);
	// left
	Draw::Line(tl.x, tl.y, tl.x, br.y, color);

	Draw::Text(tl.x + 14, tl.y, m_title.data(), Font::Get().MenuText, Color(0, 0, 0, 255), false, true);

	m_controlheight = 0;
	for (auto o : Objects)
	{
		o->Paint(tl.x + 8, tl.y + 8, &m_controlheight);
	}
}


void CChild::Click()
{
	for (auto o : Objects)
	{
		o->Click();
	}
}

void CChild::HandleInput(WPARAM vk)
{
	for (auto o : Objects)
	{
		o->HandleInput(vk);
	}
}

//--------------------------------------------------------------------------------

CSpacing::CSpacing(int height)
	: m_height(height)
{}

void CSpacing::Paint(int t_x, int t_y, float* controlheight)
{
	if (controlheight == nullptr) return;
	*controlheight += m_height;
}


//--------------------------------------------------------------------------------

CLabel::CLabel(std::wstring text, HFont font, Color color, bool centered)
	: m_text(text), m_font(font), m_color(color), m_centered(centered)
{}

void CLabel::Paint(int t_x, int t_y, float* controlheight)
{
	if (controlheight == nullptr) return;
	t_y += *controlheight;
	Draw::Text(t_x + m_x, t_y + m_y, m_text.data(), m_font, m_color, m_centered);
	*controlheight += Draw::GetTextSize(m_text.data(), m_font).y;
}

CCheckBox::CCheckBox(std::wstring label, bool* value)
	: m_label(label), m_val(value)
{}

void CCheckBox::Paint(int t_x, int t_y, float* controlheight)
{
	if (controlheight == nullptr) return;

	const auto size = 12;

	t_y += *controlheight;

	Vector2D tl(t_x + m_x, t_y + m_y);
	Vector2D br(t_x + m_x + size, t_y + m_y + size);

	int cur_x, cur_y;
	I::MatSystemSurface->SurfaceGetCursorPos(cur_x, cur_y);

	m_hover = Math::InBox(tl.x, tl.y, br.x, br.y, cur_x, cur_y);

	Color border_top, border_bottom, top, bottom;

	if (m_hover)
	{
		border_top = Color(160, 160, 160, 255);
		border_bottom = Color(109, 109, 109, 255);
		top = Color(234, 234, 234, 255);
		bottom = Color(156, 156, 156, 255);
	}
	else
	{
		border_top = Color(187, 187, 187, 255);
		border_bottom = Color(84, 84, 84, 255);
		top = Color(255, 255, 255, 255);
		bottom = Color(216, 216, 216, 255);
	}

	Draw::FilledGradientRectangle(tl.x, tl.y, br.x, br.y, border_top, border_bottom);
	Draw::FilledGradientRectangle(tl.x + 1, tl.y + 1, br.x - 1, br.y - 1, top, bottom);
	if (m_val != nullptr && *m_val)
		Draw::Text(tl.x + (size / 2) + 1, tl.y + (size / 2), L"✔", Font::Get().MenuText, Color(248, 146, 233, 255), true); /* pink */
	//Draw::Text(br.x + 4, tl.y - 1, m_label.data(), Font::Get().MenuText, Color(0, 0, 0, 255));
	Draw::Text(br.x + 4, tl.y - 1, m_label.data(), Font::Get().MenuText, Color(0,0,0,255));
	*controlheight += size + MENU_PADDING;
}

void CCheckBox::Click()
{
	if (m_val != nullptr && m_hover)
		*m_val = !*m_val;
}

template<typename T>
inline CSlider<T>::CSlider(std::wstring label, T min, T max, T* value)
	: m_label(label), m_min(min), m_max(max), m_val(value)
{}

template<typename T>
inline void CSlider<T>::Paint(int t_x, int t_y, float* controlheight)
{
	if (controlheight == nullptr) return;
	t_y += *controlheight;

	const auto width = 80;
	const auto height = 14;

	Vector2D tl(t_x + m_x, t_y + m_y);
	Vector2D br(t_x + m_x + width, t_y + m_y + height);

	int cur_x, cur_y;
	I::MatSystemSurface->SurfaceGetCursorPos(cur_x, cur_y);

	m_hover = Math::InBox(tl.x - 4, tl.y, br.x + 4, br.y, cur_x, cur_y);

	int valueX = tl.x + ((*m_val - m_min) * width / (m_max - m_min));

	if (m_val != nullptr && m_hover && InputSys::Get().IsKeyDown(VK_LBUTTON))
		*m_val = (cur_x - tl.x) * (m_max - m_min) / width;

	if (*m_val < m_min) *m_val = m_min;
	if (*m_val > m_max) *m_val = m_max;

	Draw::FilledRectangle(tl.x, tl.y + 3, br.x, br.y - 3, Color(237, 237, 237, 255));
	Draw::FilledRectangle(tl.x, tl.y + 3, valueX, br.y - 3, Color(248, 146, 233, 255));
	Draw::Rectangle(tl.x, tl.y + 3, br.x, br.y - 3, Color(141, 141, 141, 255));

	Draw::FilledGradientRectangle(valueX - 4, tl.y + 1, valueX + 4, br.y - 1, Color(208, 208, 208, 255), Color(125, 125, 125, 255));
	Draw::FilledGradientRectangle(valueX - 3, tl.y + 2, valueX + 3, br.y - 2, Color(255, 255, 255, 255), Color(187, 187, 187, 255));

	wchar_t buffer[30];
	swprintf(buffer, 30, L"%s (%s)", m_label.data(), Utilities::to_wstring(*m_val, 2).data());
	Draw::Text(br.x + 9, tl.y, buffer, Font::Get().MenuText, Color(0, 0, 0, 255));
	*controlheight += height + MENU_PADDING;
}

template class CSlider<int>;
template class CSlider<float>;
template class CSlider<double>;

//--------------------------------------------------------------------------------

CButton::CButton(int width, int height, std::wstring label, std::function<void()> cb)
	: CObject(0, 0, width, height), m_label(label), m_cb(cb)
{}

void CButton::Paint(int t_x, int t_y, float* controlheight)
{
	if (controlheight == nullptr) return;
	t_y += *controlheight;

	Vector2D tl(t_x + m_x, t_y + m_y);
	Vector2D br(t_x + m_x + m_width, t_y + m_y + m_height);

	int cur_x, cur_y;
	I::MatSystemSurface->SurfaceGetCursorPos(cur_x, cur_y);

	m_hover = Math::InBox(tl.x, tl.y, br.x, br.y, cur_x, cur_y);

	Color border_top, border_bottom, top, bottom;

	if (m_hover && InputSys::Get().IsKeyDown(VK_LBUTTON))
	{
		border_top = Color(140, 140, 140, 255);
		border_bottom = Color(188, 188, 188, 255);
		top = Color(160, 160, 160, 255);
		bottom = Color(247, 247, 247, 255);
	}
	else if (m_hover)
	{
		border_top = Color(191, 191, 191, 255);
		border_bottom = Color(144, 144, 144, 255);
		top = Color(250, 250, 250, 255);
		bottom = Color(178, 178, 178, 255);
	}
	else
	{
		border_top = Color(187, 187, 187, 255);
		border_bottom = Color(144, 144, 144, 255);
		top = Color(251, 251, 251, 255);
		bottom = Color(207, 207, 207, 255);
	}

	Draw::FilledGradientRectangle(tl.x, tl.y, br.x, br.y, border_top, border_bottom);
	Draw::FilledGradientRectangle(tl.x + 1, tl.y + 1, br.x - 1, br.y - 1, top, bottom);

	Draw::Text(tl.x + (m_width / 2), tl.y + (m_height / 2), m_label.data(), Font::Get().MenuText, Color(0, 0, 0, 255), true);
	*controlheight += m_height + MENU_PADDING;
}

void CButton::Click()
{
	if (m_hover)
		m_cb();
}

CTextBox::CTextBox(int width, int height, std::wstring placeholder, std::wstring* value)
	: CObject(0, 0, width, height), m_placeholder(placeholder), m_val(value)
{}

void CTextBox::Paint(int t_x, int t_y, float* controlheight)
{
	if (controlheight == nullptr) return;
	t_y += *controlheight;

	m_tl = Vector2D(t_x, t_y);
	m_br = Vector2D(t_x + m_width, t_y + m_height);

	int cur_x, cur_y;
	I::MatSystemSurface->SurfaceGetCursorPos(cur_x, cur_y);

	m_hover = Math::InBox(m_tl.x, m_tl.y, m_br.x, m_br.y, cur_x, cur_y);

	Color border_top, border_bottom;

	if (m_hover)
	{
		border_top = Color(160, 160, 160, 255);
		border_bottom = Color(109, 109, 109, 255);
	}
	else
	{
		border_top = Color(187, 187, 187, 255);
		border_bottom = Color(84, 84, 84, 255);
	}

	Draw::FilledGradientRectangle(m_tl.x, m_tl.y, m_br.x, m_br.y, border_top, border_bottom);
	Draw::FilledRectangle(m_tl.x + 1, m_tl.y + 1, m_br.x - 1, m_br.y - 1, Color(248, 248, 248, 255));

	if (m_val != nullptr && m_val->length() > 0)
	{
		Draw::Text(m_tl.x + 4, m_tl.y + (m_height / 2), m_val->data(), Font::Get().MenuText, Color(0, 0, 0, 255), false, true);

		if (m_selected && (int)(I::EngineClient->GetTimescale() * 10) % 2 == 0)
		{
			auto TextSize = Draw::GetTextSize(m_val->data(), Font::Get().MenuText);
			auto tl = m_tl + Vector2D(TextSize.x, 0.0f);
			auto br = m_br + Vector2D(TextSize.x, 0.0f);

			Draw::FilledRectangle(tl.x + 4, tl.y + 3, tl.x + 6, br.y - 3, Color(0, 0, 0, 255));
		}
	}
	else if (!m_selected)
	{
		Draw::Text(m_tl.x + 4, m_tl.y + (m_height / 2), m_placeholder.data(), Font::Get().MenuText, Color(100, 100, 100, 255), false, true);
	}
	else if (m_selected && (int)(I::EngineClient->GetTimescale() * 2) % 2 == 0)
	{
		Draw::FilledRectangle(m_tl.x + 4, m_tl.y + 3, m_tl.x + 6, m_br.y - 3, Color(0, 0, 0, 255));
	}
	*controlheight += m_height + MENU_PADDING;
}

void CTextBox::Click()
{
	m_selected = m_hover;
}

void CTextBox::HandleInput(WPARAM vk)
{
	if (m_val != nullptr && m_selected)
	{
		switch (vk)
		{
		case VK_BACK:  // backspace.
			if (m_val->size() > 0)
				m_val->pop_back();
			break;
		case VK_TAB:  // tab.
			break;
		case 0x0A:  // linefeed.
			break;
		case VK_RETURN:  // carriage return.
			m_selected = false;
			break;
		case VK_ESCAPE:  // escape.
			break;
		case VK_SPACE:  // space.
			*m_val += L" ";
			break;

		default:
			*m_val += (wchar_t)vk;
		}
	}
}

//--------------------------------------------------------------------------------

CCombo::CCombo(int* value, std::vector<std::wstring> labels)
	: m_val(value), m_labels(labels)
{}

void CCombo::Paint(int t_x, int t_y, float* controlheight)
{
	if (controlheight == nullptr) return;
	t_y += *controlheight;

	const auto width = 80;
	const auto height = 14;

	// root vectors
	Vector2D r_tl(t_x + m_x, t_y + m_y);
	Vector2D r_br(t_x + m_x + width, t_y + m_y + height);

	int cur_x, cur_y;
	I::MatSystemSurface->SurfaceGetCursorPos(cur_x, cur_y);

	//Draw::FilledRectangle(r_tl, r_br, COLORCODE(MENU_COLOR_CONTROL[0] - 30));
	Color color = MENU_COLOR_CONTROL;

	// Extract the red component and subtract 30, ensuring it doesn't go below 0
	int r = std::max(((color.r() >> 24) & 0xFF) - 30, 0);
	int g = (color.g() >> 16) & 0xFF;
	int b = (color.b() >> 8) & 0xFF;
	int a = color.a() & 0xFF;

	// Create the new color code
	Color newColor = Color(r, g, b, a);
	Draw::FilledRectangle(r_tl, r_br, newColor);
	Draw::Rectangle(r_tl, r_br, MENU_COLOR_CONTROL_BORDER);

	if (m_val != nullptr)
	{
		{ // Left button
			Vector2D tl(r_tl.x, r_tl.y);
			Vector2D br(r_tl.x + height, r_br.y);
			m_lhover = Math::InBox(tl, br, cur_x, cur_y);
			Color color;
			if (m_lhover && InputSys::Get().IsKeyDown(VK_LBUTTON)) color = MENU_COLOR_CONTROL_ACTIVE;
			else if (m_lhover) color = MENU_COLOR_CONTROL_HOVER;
			else color = MENU_COLOR_CONTROL;
			Draw::FilledRectangle(tl, br, color);
			Draw::Rectangle(tl, br, MENU_COLOR_CONTROL_BORDER);
			Draw::Text(tl.x + (height / 2), tl.y + (height / 2), L"◄", Font::Get().MenuText, Color(255, 255, 255, 255), true);
		}

		{ // Right button
			Vector2D tl(r_tl.x + width - height, r_tl.y);
			Vector2D br(r_tl.x + width, r_br.y);
			m_rhover = Math::InBox(tl, br, cur_x, cur_y);
			Color color;
			if (m_rhover && InputSys::Get().IsKeyDown(VK_LBUTTON)) color = MENU_COLOR_CONTROL_ACTIVE;
			else if (m_rhover) color = MENU_COLOR_CONTROL_HOVER;
			else color = MENU_COLOR_CONTROL;
			Draw::FilledRectangle(tl, br, color);
			Draw::Rectangle(tl, br, MENU_COLOR_CONTROL_BORDER);
			Draw::Text(tl.x + (height / 2) + 1, tl.y + (height / 2), L"►", Font::Get().MenuText, Color(255, 255, 255, 255), true);
		}

		Draw::Text(r_tl.x + (width / 2), r_tl.y + (height / 2), m_labels.at(*m_val).data(), Font::Get().MenuText, Color(255, 255, 255, 255), true);
	}

	*controlheight += height + MENU_PADDING;
}

void CCombo::Click()
{
	if (m_val == nullptr) return;
	if (m_lhover && *m_val > 0) *m_val -= 1;
	else if (m_rhover && *m_val >= 0 && (size_t)*m_val < m_labels.size() - 1) *m_val += 1;

}

//--------------------------------------------------------------------------------