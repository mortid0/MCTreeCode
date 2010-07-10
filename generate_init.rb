#!/usr/bin/ruby
require 'pp'
def parse_command_line argv
	params = Hash.new
	argv.each{|opt|
		line = /^(.*)*=(.*)$/.match(opt)
		params[line[1]] = line[2]
	}
	params
end

params = parse_command_line ARGV
pp params
File.open("params.dat","w+"){|file|
	PP::pp(params, file, 50)
	file.puts "v = sqrt(M/r)"
}
pp params["NP"]
NP=params["NP"].to_i
M=params["M"].to_f			#Solar mass
m=params["m"].to_f		#Earth mass
R = params["R"].to_f			#AU
disp = params["disp"].to_f	#Vel dispersion
r_a = params["r_a"].to_f		#Earth radius
r_b = params["r_b"].to_f		#Earth radius


v_0 = Math::sqrt(M/R)
mass = Array.new(NP){1}
tot_mass = 0.0
dens = Array.new(NP){1}
h = Array.new(NP){0.0002}
vel = Array.new(NP)
rad = Array.new(NP)
w = 1.0*Math::sqrt(M/(R**3))
hpi = Math::PI*0.5
NP.times{|i|
    rad[i] = Array.new(3)
    phi = 2.0*Math::PI * rand()
    eta = 2.0*Math::PI * rand()
    rad[i][0] = (R + r_a*Math::cos(eta))*Math::cos(phi)
    rad[i][1] = (R + r_a*Math::cos(eta))*Math::sin(phi)
    rad[i][2] = r_b * Math::sin(eta)
    r = Math::sqrt(rad[i][0]**2 + rad[i][1]**2 + rad[i][2]**2)
    vel[i] = Array.new(3)
	tot_mass += mass[i]
    v = Math::sqrt(M/r)
#    v = w*r
	cd = disp*v*rand()
	phi_disp = 2.0*Math::PI*rand()
	eta_disp = 2.0*Math::PI*rand()
    vel[i][0] = v*Math::cos(hpi + phi) + cd*Math::cos(phi_disp)*Math::cos(eta_disp)
    vel[i][1] = v*Math::sin(hpi + phi) + cd*Math::cos(phi_disp)*Math::sin(eta_disp)
    vel[i][2] = 0.0 + cd*Math::sin(phi_disp)
}
=begin /barnes style input data generation
File.open("input.data","w+"){|file|
	file.puts "#{(NP+1)}"
	file.puts "3"
	file.puts "0.0"
	NP.times{|i| file.puts mass[i]*m/tot_mass}
	file.puts M
	NP.times{|i| file.puts "#{rad[i][0]} #{rad[i][1]} #{rad[i][2]}"}
	file.puts "0.0 0.0 0.0"
	NP.times{|i| file.puts "#{vel[i][0]} #{vel[i][1]} #{vel[i][2]}"}
	file.puts "0.0 0.0 0.0"
}
=end
File.open("input.data","w+"){|file|
	file.puts "#{(NP+1)}"
	file.puts "3"
	file.puts "0.0"
	NP.times{|i| 
		file.puts "#{rad[i][0]} #{rad[i][1]} #{rad[i][2]} #{vel[i][0]} #{vel[i][1]} #{vel[i][2]} #{m/NP} #{dens[i]} #{h[i]}"
	}
	file.puts "0.0 0.0 0.0 0.0 0.0 0.0 #{M} 10 0.002"
}
