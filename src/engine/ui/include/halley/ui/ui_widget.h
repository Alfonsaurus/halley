#pragma once
#include "ui_sizer.h"
#include "ui_root.h"
#include "ui_painter.h"
#include "halley/maths/vector2.h"
#include "halley/maths/rect.h"
#include "halley/maths/vector4.h"
#include "halley/data_structures/maybe.h"
#include "ui_input.h"
#include "ui_data_bind.h"
#include "ui_style.h"
#include "halley/core/api/audio_api.h"

namespace Halley {
	class UIEvent;
	class UIValidator;
	class UIDataBind;
	class UIAnchor;
	class UIBehaviour;

	enum UIWidgetUpdateType {
		First,
		Full,
		Partial
	};

	class UIWidget : public IUIElement, public UIParent, public IUISizer, public std::enable_shared_from_this<UIWidget> {
		friend class UIParent;
		friend class UIRoot;

	public:
		UIWidget(String id = "", Vector2f minSize = {}, std::optional<UISizer> sizer = {}, Vector4f innerBorder = {});
		virtual ~UIWidget();

		void doDraw(UIPainter& painter) const;
		void doUpdate(UIWidgetUpdateType updateType, Time t, UIInputType inputType, JoystickType joystickType);
		void doRender(RenderContext& rc);

		Vector2f getLayoutMinimumSize(bool force) const override;
		void setRect(Rect4f rect) override;

		UIRoot* getRoot() override;
		const UIRoot* getRoot() const override;
		UIParent* getParent() const;

		void layout();

		virtual void alignAt(const UIAnchor& anchor);
		void alignAtAnchor();
		void setAnchor(UIAnchor anchor);
		void setAnchor();

		virtual std::optional<UISizer>& tryGetSizer();
		virtual UISizer& getSizer();

		void add(std::shared_ptr<IUIElement> element, float proportion = 0, Vector4f border = Vector4f(), int fillFlags = UISizerFillFlags::Fill) override;
		void addSpacer(float size) override;
		void addStretchSpacer(float proportion = 0) override;

		void clear() override;

		virtual bool canInteractWithMouse() const;
		virtual bool isFocusLocked() const;
		bool isMouseOver() const;
		bool isFocused() const;

		void setId(const String& id);
		const String& getId() const override;

		Vector2f getPosition() const;
		virtual Vector2f getLayoutOriginPosition() const;
		Vector2f getSize() const;
		Vector2f getMinimumSize() const;
		Vector4f getInnerBorder() const;
		Rect4f getRect() const override;
		virtual bool ignoreClip() const;

		void setPosition(Vector2f pos);
		void setMinSize(Vector2f size);
		void setInnerBorder(Vector4f border);

		void setFocused(bool focused);
		void setMouseOver(bool mouseOver);
		virtual void pressMouse(Vector2f mousePos, int button);
		virtual void releaseMouse(Vector2f mousePos, int button);
		virtual void onMouseOver(Vector2f mousePos);
		virtual Rect4f getMouseRect() const;

		bool isActive() const override;
		void setActive(bool active);
		bool isEnabled() const;
		void setEnabled(bool enabled);
		bool isModal() const;
		void setModal(bool modal);
		bool isMouseBlocker() const;
		void setMouseBlocker(bool blocker);
		bool shrinksOnLayout() const;
		void setShrinkOnLayout(bool shrink);

		bool isAlive() const;
		void destroy();
		void forceDestroy();

		void setEventHandler(std::shared_ptr<UIEventHandler> handler);
		UIEventHandler& getEventHandler();
		void setHandle(UIEventType type, UIEventCallback handler);
		void setHandle(UIEventType type, String id, UIEventCallback handler);

		void setCanSendEvents(bool canSend);

		virtual void setInputType(UIInputType uiInput);
		virtual void setJoystickType(JoystickType joystickType);
		void setOnlyEnabledWithInputs(const std::vector<UIInputType>& inputs);
		const std::vector<UIInputType>& getOnlyEnabledWithInput() const;
		virtual void setInputButtons(const UIInputButtons& buttons);
		UIInputType getLastInputType() const;

		void setValidator(std::shared_ptr<UIValidator> validator);
		std::shared_ptr<UIValidator> getValidator() const;
		virtual void onValidatorSet();

		UIDataBind::Format getDataBindFormat() const;
		void setDataBind(std::shared_ptr<UIDataBind> dataBind);
		std::shared_ptr<UIDataBind> getDataBind() const;
		virtual void readFromDataBind();
		void bindData(const String& childId, bool initialValue, UIDataBindBool::WriteCallback callback = {});
		void bindData(const String& childId, int initialValue, UIDataBindInt::WriteCallback callback = {});
		void bindData(const String& childId, float initialValue, UIDataBindFloat::WriteCallback callback = {});
		void bindData(const String& childId, const String& initialValue, UIDataBindString::WriteCallback callback = {});

		bool isDescendentOf(const UIWidget& ancestor) const override;
		void setMouseClip(std::optional<Rect4f> mouseClip, bool force);

		virtual void onManualControlCycleValue(int delta);
		virtual void onManualControlAnalogueAdjustValue(float delta, Time t);
		virtual void onManualControlActivate();

		UIInput::Priority getInputPriority() const;

		void setChildLayerAdjustment(int delta);
		int getChildLayerAdjustment() const;
		void setNoClipChildren(bool noClip);
		bool getNoClipChildren() const;

		void sendEvent(UIEvent&& event) const override;
		void sendEventDown(const UIEvent& event) const;
		void forceAddChildren(UIInputType inputType, bool forceRecursive);

		void addBehaviour(std::shared_ptr<UIBehaviour> behaviour);
		void clearBehaviours();
		const std::vector<std::shared_ptr<UIBehaviour>>& getBehaviours() const;

		std::optional<AudioHandle> playSound(const String& eventName);

		bool needsLayout() const;
		void markAsNeedingLayout() override;

		virtual bool canReceiveFocus() const;

		virtual void onAddedToRoot();

	protected:
		virtual void draw(UIPainter& painter) const;
		virtual void drawAfterChildren(UIPainter& painter) const;
		virtual void drawChildren(UIPainter& painter) const;
		virtual void render(RenderContext& rc) const;
		virtual void update(Time t, bool moved);

		void updateBehaviours(Time t);

		virtual void onFocus();
		virtual void onFocusLost();
		virtual void onLayout();
		virtual void onDestroyRequested();
		virtual void onParentChanged();

		void notifyDataBind(bool data) const;
		void notifyDataBind(int data) const;
		void notifyDataBind(float data) const;
		void notifyDataBind(const String& data) const;

		void shrink();
		void forceLayout();

		virtual void onInput(const UIInputResults& input, Time time);
		virtual void updateInputDevice(const InputDevice& inputDevice);

		virtual void onEnabledChanged();

		virtual void checkActive();

		UIInputType lastInputType = UIInputType::Undefined;

	private:
		void setParent(UIParent* parent);
		void notifyTreeAddedToRoot();

		void setWidgetRect(Rect4f rect);
		void resetInputResults();
		void updateActive(bool wasActiveBefore);

		UIParent* parent = nullptr;
		String id;

		std::vector<UIInputType> onlyEnabledWithInputs;
		std::unique_ptr<UIInputButtons> inputButtons;
		UIInputResults inputResults;

		Vector2f position;
		Vector2f size;
		Vector2f minSize;
		std::optional<Rect4f> mouseClip;

		Vector4f innerBorder;
		std::optional<UISizer> sizer;

		mutable Vector2f layoutSize;
		mutable int layoutNeeded = 1;

		std::shared_ptr<UIEventHandler> eventHandler;
		std::shared_ptr<UIValidator> validator;
		std::shared_ptr<UIDataBind> dataBind;
		std::unique_ptr<UIAnchor> anchor;
		std::vector<std::shared_ptr<UIBehaviour>> behaviours;

		int childLayerAdjustment = 0;

		bool activeByUser = true;
		bool activeByInput = true;
		bool enabled = true;
		bool alive = true;
		bool focused = false;
		bool mouseOver = false;
		bool positionUpdated = false;
		bool modal = true;
		bool mouseBlocker = true;
		bool shrinkOnLayout = true;
		bool destroying = false;
		bool canSendEvents = true;
		bool dontClipChildren = false;
	};

	template <typename F>
	void UIParent::descend(F f)
	{
		for (auto& c: children) {
			f(c);
			c->descend(f);
		}
	}
}
