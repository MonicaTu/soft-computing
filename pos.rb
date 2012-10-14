c1 = 1
c2 = 1
r1 = rand
r2 = rand
n = 100
t = 100
v = [0, 0]
max = 100
#max = 4

PI = 3.141592654

z_gbest = 100 * 100 + 100 * 100
pos_pbest = [0, 0]
pos_curr = [0, 0]

particle = Array.new(n)

def sphere(x, y)
   result = x**2 + y**2 
   return result
end

def rastrigin(x, y)
   result = (x**2-10*Math.cos(2*PI*x)+10) + (y**2-10*Math.cos(2*PI*y)+10) 
   return result
end

for i in 1..n
    x = rand(max*2)-max
    y = rand(max*2)-max
    z = sphere(x, y) 
#    z = rastrigin(x, y) 
    pos_new = [x, y]
    pos_pbest[0] = pos_curr[0] 
    pos_pbest[1] = pos_curr[1] 
    particle[i] = [v, pos_new, pos_pbest, z]
    
    if (z < z_gbest)
        pos_gbest = pos_new
    end
end

for j in 1..t 
    file = File.open("data/sphere/data#{j}.txt", 'wb+')
#    file = File.open("data/rastrigin/data#{j}.txt", 'wb+')
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
        z = sphere(pos_curr[0], pos_curr[1])
#        z = rastrigin(pos_curr[0], pos_curr[1])
        if (z < particle[i][3])
            particle[i][2] = pos_curr
            particle[i][3] = z
        end
        
        # update gbest
        if (z < z_gbest)
            pos_gbest = particle[i][1]
        end
    
        file << "#{particle[i][1][0]} #{particle[i][1][1]}\n"
        
    end
    file.close
end
