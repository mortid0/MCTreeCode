#!/usr/bin/ruby

num_files = ARGV[0].to_i          # 12
step = ARGV[1].to_i				 # 8
nb=ARGV[2].to_i					 # 127
mass = Array.new(nb)
rad = Array.new(nb)
vel = Array.new(nb)

num_files.times{|file_nm|
	file_num = file_nm
	str = "0"*(4-(file_num*step).to_s.size)+(file_num*step).to_s
	puts str
	File.open("run_#{str}.data","r"){|input|
		nb = input.readline.to_i
		input.readline #dim
		input.readline #time
		mass = Array.new(nb){input.readline.to_f}
		rad = Array.new(nb){input.readline.split.collect{|o| o.to_f} }
		vel = Array.new(nb){input.readline.split.collect{|o| o.to_f} }
	}
	File.open("8#{str}.dump", "w+"){|output|
#		output.puts "# mass R_x R_y R_z v_x v_y v_z"
		nb.times{|i|
			output.puts "#{mass[i]}; #{rad[i][0]}; #{rad[i][1]}; #{rad[i][2]}; #{vel[i][0]}; #{vel[i][1]}; #{vel[i][2]};\n"
		}
	}
}

