require "./sphere.rb"
#require "./rastrigin.rb"

c1 = 2
c2 = 2
r1 = rand
r2 = rand
n = 50
t = 100
v = [0, 0]
max = 100 # sphere
#max = 4 # rastrigin 

puts "r1:#{r1} r2:#{r2}"

PI = 3.141592654

pso_pbest = [0, 0]
pso_curr = [0, 0]

particle = Array.new(n)

for i in 1..n
    x = rand(max*2)-max
    y = rand(max*2)-max

    z_gbest = fn(max, max) 

    z = fn(x, y) 
    pso_new = [x, y]
    pso_pbest[0] = pso_curr[0] 
    pso_pbest[1] = pso_curr[1] 
    particle[i] = [v, pso_new, pso_pbest, z]
    
    if (z < z_gbest)
        pso_gbest = pso_new
    end
end

for j in 1..t 
#    file = File.open("data/sphere/data#{j}.txt", 'wb+')
#    file = File.open("data/rastrigin/data#{j}.txt", 'wb+')
#    file = File.open("data/fn/data#{j}.txt", 'wb+')
    for i in 1..n
        # update v
        # v = c1*r1*(pso_pbest-pso_curr) + c2*r2*(pso_gbest-pso_curr)
        pso_curr = particle[i][1]
        v[0] = c1*r1*(pso_pbest[0]-pso_curr[0]) + c2*r2*(pso_gbest[0]-pso_curr[0])
        v[1] = c1*r1*(pso_pbest[1]-pso_curr[1]) + c2*r2*(pso_gbest[1]-pso_curr[1])
    
        # update pso_curr 
        particle[i][1][0] = particle[i][1][0] + v[0]
        particle[i][1][1] = particle[i][1][1] + v[1]
    
        # update pso_pbest
        z = fn(pso_curr[0], pso_curr[1])
        if (z < particle[i][3])
            particle[i][2] = pso_curr
            particle[i][3] = z
        end
        
        # update gbest
        if (z < z_gbest)
            pso_gbest = particle[i][1]
            z_gbest = z
        end
    
#        file << "#{particle[i][1][0]} #{particle[i][1][1]}\n"

        p "(i#{j},p#{i}) #{z_gbest} at #{pso_gbest}"
    end
#    file.close
end
