#!/usr/bin/env ruby
module Screen
  def self.clear
    print "\e[2J\e[f"
  end
end

time = ''
print "Put time here: "
time = gets.chomp
# time  = "0:30:00"
array = time.split(":")

if array.length == 1
  hour = 0
  min = array[0].to_i
  sec = 0
elsif array.length == 2
  hour = array[0].to_i
  min  = array[1].to_i
  sec  = 0
elsif array.length == 3
  hour = array[0].to_i
  min  = array[1].to_i
  sec  = array[2].to_i  
end

duration = (hour)*3600 + (min)*60 + sec

# if clear does not work then use system call to clear screen
duration.downto(0) do |n| 
  sleep(1)
  Screen.clear
  puts "#{n/3600}:#{(n%3600)/60}:#{(n%3600)%60}"
end

# http://www.computerhope.com/issues/ch001041.htm
# http://hints.macworld.com/article.php?story=20011108211802830
