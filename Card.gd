extends Node2D

# Contains the id of the card
# A deck contains 52 cards maximum
export (int, 0, 51, 1) var id = 0

# If TRUE, shows the face
export (bool) var flipped = false

func _ready():
	if flipped:
		show_face()
	else:
		show_back()

func reset():
	$Back.visible = false
	$Color.visible = false
	$Ace.visible = false
	$Number.visible = false
	$HeadBlack.visible = false
	$HeadRed.visible = false

func show_back():
	$Back.visible = true

func show_face():
	var color = Deck.getColor(id)
	var value = Deck.getValue(id)
	
	# Set color
	$Color.visible = true
	$Color.frame = color
	
	# Set ace
	if Deck.isAce(id):
		$Ace.visible = true
		$Ace.frame = color
	
	# Set number
	if Deck.isNumber(id):
		$Number.visible = true
		$Number.frame_coords.x = value - 1
		if Deck.isBlackColor(id):
			$Number.frame_coords.y = 1
	
	# Set head
	if Deck.isHead(id):
		var head = null
		if Deck.isBlackColor(id):
			head = $HeadBlack
		else:
			head = $HeadRed
		head.visible = true
		head.frame_coords.y = value - 10
