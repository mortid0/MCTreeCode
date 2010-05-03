#!/usr/bin/ruby

exp_num = 0
#iterate over 5 diff. params to make runs
#default params
#NP = 8191			#Particle number
NP=131071
M = 1.0			#Solar mass
m = M*10**(-1)		#Earth mass
R = 1.0			#AU
disp = 0.0		#Vel dispersion
r_a = 0.05		#Earth radius(width, along R)
r_b = 0.01		#Earth radius(height(
exp_dir = Dir.pwd.to_s
# m
5.times{|i|
	Dir.chdir("#{exp_dir}")
	Dir.mkdir("#{exp_num}")
	`cp generate_init.rb #{exp_num}`
	`cp main #{exp_num}`
	`cp fixpgplotnames.bash #{exp_num}`
	`cp splash.defaults #{exp_num}`
	`cp splash.columns #{exp_num}`
	curr_dir = exp_dir + "/" + exp_num.to_s
	puts "#{exp_dir} #{curr_dir}"
	Dir.chdir("#{curr_dir}")
	Dir.mkdir("dump")
	`./generate_init.rb #{NP} #{m*(2**i)} #{disp} #{r_a} #{r_b}`
	`./main input.data 0.0`
	`asplash dump/*.dump -x 2 -y 3 -dev /png`
	`./fixpgplotnames.bash`
#	`rm *.dump`
	exp_num += 1
}

