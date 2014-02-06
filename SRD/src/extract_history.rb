# extract_history.rb
# -------------------
# Find all commits with a message starting with {HIST} for a given file.
# Prints a LaTeX tabular that can be used as changelog.

require 'time'

if ARGV.empty?
	$stderr.puts "USAGE: #{$0} FILENAME"
	exit 1
end

LOG_REGEXP = /commit\s*([\da-f]+)\nAuthor:\s*([^<]+).*\nDate:\s*([^\n]+)[\n\s\t]*\{HIST\}([^\n]+)/

puts "%Created with extract_history.rb"

log = []
`git log #{ARGV[0]}`.gsub LOG_REGEXP do |match|
	commit = "\\textbf{#{$1[0..8]}}"
	date = Time.parse($3).strftime('%d/%m/%Y')
	author = $2
	description = $4
	log << [commit, date, author, description]
end

unless log.empty?
	puts "\\begin{tabular}{| r | c | l | l |}"
	puts "\\hline"
	puts "Commit & Date & Auteur & Description\\\\"
	puts "\\hline"

	#Log is in reverse chronological order
	log.reverse.each do |line|
		puts line * ' & ' + "\\\\"
	end
	
	puts "\\hline"
	puts "\\end{tabular}"
end
	