extends Node

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

# Get color from the card
func getColor(id):
	return round(id / 13)

# Returns TRUE if the color is black
func isBlackColor(id):
	var color = getColor(id)
	return color == Deck.Colors.SPADE or color == Deck.Colors.CLUB

# Returns TRUE if the color is red
func isRedColor(id):
	var color = getColor(id)
	return color == Deck.Colors.HEART or color == Deck.Colors.DIAMOND

# Get the current card number in the suite
func getValue(id):
	return id % 13

# Returns TRUE if the card is an ace
func isAce(id):
	return getValue(id) == 0

# Returns TRUE if the value is greater than
func isNumber(id):
	var value = getValue(id)
	return 0 < value and value < 10

# Returns TRUE if the card is head
func isHead(id):
	return getValue(id) >= 10
