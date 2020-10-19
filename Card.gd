extends Node2D

# Contains the id of the card
# A deck contains 52 cards maximum
export (int, 0, 51, 1) var id = 0

func _ready():
	var color = Deck.getColor(id)
	var value = Deck.getValue(id)
	
	# Set color
	$Color.frame = color
	
	# Set ace
	if Deck.isAce(id):
		$Ace.visible = true
		$Ace.frame = color
	
	# Set number
	if Deck.isNumber(id):
		$Number.visible = true
		$Number.frame_coords.x = value - 1
		if color == Deck.Colors.SPADE or color == Deck.Colors.CLUB:
			$Number.frame_coords.y = 1
	
	# Set head
	if Deck.isHead(id):
		var head = null
		if color == Deck.Colors.SPADE or color == Deck.Colors.CLUB:
			head = $HeadBlack
		else:
			head = $HeadRed
		head.visible = true
		head.frames_coord.x = value - 10
