extends KinematicBody2D

const Colors = {
	SPADE = 0,
	HEART = 1,
	CLUB = 2,
	DIAMOND = 3,
}

const Heads = {
	JACK = 10,
	QUEEN = 11,
	KING = 12,
}

# Contains the id of the card
# A deck contains 52 cards maximum
export (int, 0, 51, 1) var id = 0 setget set_identifier
var color = 0
var value = 0

# If TRUE, shows the face
export (bool) var flipped = false setget set_flipped

# If TRUE, can be flipped
export (bool) var selectable = true

# Update the card features on load
func _ready():
	update_features()

# Update the flipped status
func set_flipped(is_flipped):
	flipped = is_flipped
	update_features()

# Update the front side of the card when setting the id
func set_identifier(new_id):
	id = new_id
	color = round(id / 13)
	value = id % 13
	update_features()

# Update the child features of the card
# according to its current identifer
func update_features():
	
	# If flipped shows the back side of the card
	# If not, shows the front side
	if flipped:
		$Front.visible = false
		$Back.visible = true
	else:
		$Front.visible = true
		$Back.visible = false
	
	# Reset features
	$Front/Ace.visible = false
	$Front/Number.visible = false
	$Front/HeadBlack.visible = false
	$Front/HeadRed.visible = false
	
	# Set color
	$Front/Color.visible = true
	$Front/Color.frame = color
	
	# Set ace
	if is_ace():
		$Front/Ace.visible = true
		$Front/Ace.frame = color
	
	# Set number
	if is_number():
		$Front/Number.visible = true
		$Front/Number.frame_coords.x = value - 1
		$Front/Number.frame_coords.y = 0
		if is_black_color():
			$Front/Number.frame_coords.y = 1
	
	# Set head
	if is_head():
		var head = null
		if is_black_color():
			head = $Front/HeadBlack
		if is_red_color():
			head = $Front/HeadRed
		head.visible = true
		head.frame_coords.y = value - 10

# Toggle card if selectable
func _on_Card_input_event(_viewport, event, _shape_idx):
	if selectable and event is InputEventMouseButton:
		if event.button_index == BUTTON_LEFT and event.pressed:
			$FlipEffect.play()
			set_flipped(!flipped)

# Returns TRUE if the color is black
func is_black_color():
	return color == Colors.SPADE or color == Colors.CLUB

# Returns TRUE if the color is red
func is_red_color():
	return color == Colors.HEART or color == Colors.DIAMOND

# Returns TRUE if the card is an ace
func is_ace():
	return value == 0

# Returns TRUE if the card is head
func is_head():
	return value in Heads.values()

# Returns TRUE if the value is a classic number
func is_number():
	return not is_ace() and not is_head()
