#!/usr/bin/ruby

NP=ARGV[0].to_i		#Particle 
M=1.0			#Solar mass
m=ARGV[1].to_f		#Earth mass
R = 1.0			#AU
disp = ARGV[2].to_f		#Vel dispersion
r_a = ARGV[3].to_f		#Earth radius
r_b = ARGV[4].to_f		#Earth radius
File.open("params.dat","w+"){|file|
	file.puts "NP #{NP}\nM #{M}\nm #{m}\nR #{R}\ndisp #{disp}\nr_a #{r_a}\nr_b #{r_b}"
	file.puts "v = sqrt(M/r)"
}

v_0 = Math::sqrt(M/R)
mass = Array.new(NP){m/NP}
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
    v = Math::sqrt(M/r)
#    v = w*r
	cd = disp*v*rand()
	phi_disp = 2.0*Math::PI*rand()
	eta_disp = 2.0*Math::PI*rand()
    vel[i][0] = v*Math::cos(hpi + phi) + cd*Math::cos(phi_disp)*Math::cos(eta_disp)
    vel[i][1] = v*Math::sin(hpi + phi) + cd*Math::cos(phi_disp)*Math::sin(eta_disp)
    vel[i][2] = 0.0 + cd*Math::sin(phi_disp)
}
File.open("input.data","w+"){|file|
	file.puts "#{(NP+1)}"
	file.puts "3"
	file.puts "0.0"
	NP.times{|i| file.puts mass[i]}
	file.puts M
	NP.times{|i| file.puts "#{rad[i][0]} #{rad[i][1]} #{rad[i][2]}"}
	file.puts "0.0 0.0 0.0"
	NP.times{|i| file.puts "#{vel[i][0]} #{vel[i][1]} #{vel[i][2]}"}
	file.puts "0.0 0.0 0.0"
}
