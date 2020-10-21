extends Node

enum Combos {
	NONE,
	PAIR,
	TWO_PAIR,
	THREE_OF_A_KIND,
	STRAIGHT,
	FLUSH,
	FULL_HOUSE,
	FOUR_OF_A_KIND,
	STRAIGHT_FLUSH,
	ROYAL_FLUSH,
}

const Messages = [
	"Perdu, on retente sa chance?",
	"La paire sauve la mise!",
	"Double paire!",
	"C'est un Brelan!",
	"Une Quinte!",
	"Bien joué�, Flush!",
	"Quelle chance! Full!",
	"C'est bien un Carré�!",
	"Une Quinte Flush!",
	"Quinte Flush Royale! Woaw!",
]

const Bonus = [ 0, 1, 2, 3, 4, 5, 6, 9, 11, 21 ]

func get_combination_code(ids):
	
	# get a list of values
	var values = []
	for n in ids:
		values.append(n % 13)
	
	# get a list of colors
	var colors = []
	for n in ids:
		colors.append(round(n / 13))
	
	var pairs = 0
	var threes = 0
	var fours = 0
	
	# look for multiples in value
	var processed_values = []
	for n in values:
		if not processed_values.has(n):
			processed_values.append(n)
			var count = values.count(n)
			if count == 2:
				pairs += 1
			if count == 3:
				threes += 1
			if count == 4:
				fours += 1
	
	if fours == 1:
		return Combos.FOUR_OF_A_KIND
	
	if threes == 1 and pairs == 1:
		return Combos.FULL_HOUSE
	
	if threes == 1:
		return Combos.THREE_OF_A_KIND
	
	if pairs == 2:
		return Combos.TWO_PAIR
	
	if pairs == 1:
		return Combos.PAIR
	
	# Check if straight
	var straight = false
	
	var prev_value = null
	for n in values.sort():
		if prev_value == null or prev_value == n - 1:
			prev_value = n
			straight = true
		else:
			straight = false
			break
	
	# Check if flush
	var flush = colors.count(colors.front())
	
	if straight and flush and values.back() == 12:
		return Combos.ROYAL_FLUSH
	
	if straight and flush:
		return Combos.STRAIGHT_FLUSH
	
	if straight:
		return Combos.STRAIGHT
	
	if flush:
		return Combos.FLUSH
	
	return Combos.NONE
