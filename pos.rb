c1 = 1
c2 = 1
r1 = rand
r2 = rand
n = 100
t = 1000
v = [0, 0]

z_gbest = 100 * 100 + 100 * 100
pos_pbest = [0, 0]
pos_curr = [0, 0]

particle = Array.new(n)

for i in 1..n
    x = rand(200)-100
    y = rand(200)-100
    z = x**2 + y**2
    pos_new = [x, y]
    pos_pbest[0] = pos_curr[0] 
    pos_pbest[1] = pos_curr[1] 
    particle[i] = [v, pos_new, pos_pbest, z]
    
    if (z < z_gbest)
        pos_gbest = pos_new
    end
end

for j in 1..t 
for i in 1..n
    # update v
    # v = c1*r1*(pos_pbest-pos_curr) + c2*r2*(pos_gbest-pos_curr)
    pos_curr = particle[i][1]
    v[0] = c1*r1*(pos_pbest[0]-pos_curr[0]) + c2*r2*(pos_gbest[0]-pos_curr[0])
    v[1] = c1*r1*(pos_pbest[1]-pos_curr[1]) + c2*r2*(pos_gbest[1]-pos_curr[1])

    # update pos_curr 
    particle[i][1][0] = particle[i][1][0] + v[0]
    particle[i][1][1] = particle[i][1][1] + v[1]

    # update pos_pbest
    z = pos_curr[0]**2 + pos_curr[1]**2
    if (z < particle[i][3])
        particle[i][2] = pos_curr
        particle[i][3] = z
    end
    
    # update gbest
    if (z < z_gbest)
        pos_gbest = particle[i][1]
    end
end
end
    puts "#{particle[i][1]}"
