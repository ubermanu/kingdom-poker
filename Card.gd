extends KinematicBody2D

# Contains the id of the card
# A deck contains 52 cards maximum
export (int, 0, 51, 1) var id = 0 setget set_identifier

# If TRUE, shows the face
export (bool) var flipped = false setget set_flipped

# If TRUE, can be flipped
export (bool) var selectable = true

# Update the flipped status
# If flipped shows the back side of the card
# If not, shows the front side
func set_flipped(is_flipped):
	flipped = is_flipped
	if flipped:
		$Front.visible = false
		$Back.visible = true
	else:
		$Front.visible = true
		$Back.visible = false

# Update the front side of the card when setting the id
func set_identifier(new_id):
	id = new_id
	
	# Reset features
	$Front/Ace.visible = false
	$Front/Number.visible = false
	$Front/HeadBlack.visible = false
	$Front/HeadRed.visible = false
	
	var color = Deck.getColor(id)
	var value = Deck.getValue(id)
	
	# Set color
	$Front/Color.visible = true
	$Front/Color.frame = color
	
	# Set ace
	if Deck.isAce(id):
		$Front/Ace.visible = true
		$Front/Ace.frame = color
	
	# Set number
	if Deck.isNumber(id):
		$Front/Number.visible = true
		$Front/Number.frame_coords.x = value - 1
		$Front/Number.frame_coords.y = 0
		if Deck.isBlackColor(id):
			$Front/Number.frame_coords.y = 1
	
	# Set head
	if Deck.isHead(id):
		var head = null
		if Deck.isBlackColor(id):
			head = $Front/HeadBlack
		else:
			head = $Front/HeadRed
		head.visible = true
		head.frame_coords.y = value - 10

# Toggle card if selectable
func _on_Card_input_event(_viewport, event, _shape_idx):
	if selectable and event is InputEventMouseButton:
		if event.button_index == BUTTON_LEFT and event.pressed:
			$FlipEffect.play()
			set_flipped(!flipped)
