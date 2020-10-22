extends KinematicBody2D

export (int) var value = 1 setget set_value
export (bool) var draggable = false
export (bool) var generator = false

var dragging = false
var offset = Vector2.ZERO

var allowed_values = [
	1, 5, 10, 20, 100
]

signal bet(amount)

func _ready():
	get_tree().current_scene.connect("bank_updated", self, "_on_Game_bank_updated")
	get_tree().current_scene.connect("input", self, "_on_Game_input")

func _on_Game_bank_updated(bank):
	if generator:
		visible = bank >= value

func set_value(v):
	value = v
	$Sprite.frame = allowed_values.find(v)

func _process(_delta):
	if draggable and dragging:
		var mouse = get_viewport().get_mouse_position()
		position = Vector2(mouse.x - offset.x, mouse.y - offset.y)

func _on_Token_input_event(_viewport, event, _shape_idx):
	if draggable and event is InputEventMouseButton:
		if event.button_index == BUTTON_LEFT and event.pressed:
			offset = get_viewport().get_mouse_position() - position
			dragging = true
			$AnimationPlayer.play("Zoom")
	if generator and event is InputEventMouseButton:
		if event.button_index == BUTTON_LEFT and event.pressed:
			var new_token = duplicate()
			new_token.offset = get_viewport().get_mouse_position() - position
			new_token.draggable = true
			new_token.dragging = true
			new_token.generator = false
			new_token.add_to_group("Temp")
			new_token.get_node("AnimationPlayer").play("Zoom")
			get_tree().root.add_child(new_token)
			emit_signal("bet", value)

func _on_Game_input(event):
	if draggable and event is InputEventMouseButton:
		if event.button_index == BUTTON_LEFT and !event.pressed:
			if dragging == true:
				$DropEffect.play()
				$AnimationPlayer.play_backwards("Zoom")
			dragging = false

func _on_VisibilityNotifier2D_screen_exited():
	queue_free()

func _on_Token_mouse_entered():
	if draggable or generator:
		Input.set_default_cursor_shape(Input.CURSOR_DRAG)

func _on_Token_mouse_exited():
	if draggable or generator:
		Input.set_default_cursor_shape(Input.CURSOR_ARROW)
