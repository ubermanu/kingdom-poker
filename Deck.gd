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

# Get the current card number in the suite
func getValue(id):
	return id % 13

# Returns TRUE if the card is an ace
func isAce(id):
	return getValue(id) == 0

# Returns TRUE if the value is greater than
func isNumber(id):
	return getValue(id) < 10

# Returns TRUE if the card is head
func isHead(id):
	return getValue(id) >= 10
