extends KinematicBody2D

var dragging = false
var offset = Vector2.ZERO

signal dragsignal

func _ready():
	connect("dragsignal", self, "_set_drag_pc")

func _process(delta):
	if dragging:
		var mousepos = get_viewport().get_mouse_position()
		self.position = Vector2(mousepos.x - offset.x, mousepos.y - offset.y)

func _set_drag_pc():
	dragging = !dragging
	offset = get_viewport().get_mouse_position() - position

func _on_Token_input_event(viewport, event, shape_idx):
	if event is InputEventMouseButton:
		if event.button_index == BUTTON_LEFT and event.pressed:
			emit_signal("dragsignal")
		elif event.button_index == BUTTON_LEFT and !event.pressed:
			emit_signal("dragsignal")
	elif event is InputEventScreenTouch:
		if event.pressed and event.get_index() == 0:
			self.position = event.get_position()

func _on_VisibilityNotifier2D_screen_exited():
	queue_free()

func _on_Token_mouse_entered():
	Input.set_default_cursor_shape(Input.CURSOR_DRAG)

func _on_Token_mouse_exited():
	Input.set_default_cursor_shape(Input.CURSOR_ARROW)
