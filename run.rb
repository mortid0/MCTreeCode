#!/usr/bin/ruby

`rm *.data`
`rm *.dump`
`rm *.png*`

`./generate_init.rb`
`./treecode in=input.data nbody=1024 theta=0.7 out=run_%03d.data dtime=1/128 dtout=1/16 tstop=10`
`./convert.rb`
`asplash *.dump -x 2 -y 3 -dev /png`