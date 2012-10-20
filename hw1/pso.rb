#require "./sphere.rb"
require "./rastrigin.rb"

PI = 3.141592654
runs = 10
mean_z_data = Array.new(runs)
for k in 0..(runs-1)
p "Run #{k+1}:"
#c1 = 1
#c2 = 1
c1 = 2
c2 = 2
r1 = rand
r2 = rand
n = 100
t = 100
v = [0, 0]
#max = 100 # sphere
max = 4 # rastrigin 

#puts "r1:#{r1} r2:#{r2}"

#pos_pbest = [0, 0]
#pos_curr = [0, 0]

z_sum = 0

particle = Array.new(n)

for i in 0..(n-1)
    if (i == 0)
	x = -100
	y = -100
    elsif (i == 1)
	x = 100
	y = -100
    elsif (i == 2)
	x = -100
	y = 100
    elsif (i == 3)
	x = 100
	y = 100
    else
    	x = rand(max*2)-max
    	y = rand(max*2)-max
    end
    z = fn(x, y)

    pos_curr = [x, y]
    pos_pbest = pos_curr 
    particle[i] = [v, pos_curr, pos_pbest, z]

#    p "particle[#{i}]=#{particle[i]}"

#    if ( i == 0 )
#        z_gbest = z;
#        pos_gbest = pos_curr
#    else 
#        if ( z < z_gbest)
#            z_gbest = z;
#            pos_gbest = pos_curr
#        end
#    end
end
z_gbest = 0;
pos_gbest = [0, 0];
#p "Generateing first 50 points was done."
#p "---pos_gbest=#{pos_gbest} z_gbest=#{z_gbest}"
#gets
#tmp_gbest = [0, 0]
next_pos = [0, 0]
for j in 1..t
#    file = File.open("data/sphere/data#{j}.txt", 'wb+')
#    file = File.open("data/rastrigin/data#{j}.txt", 'wb+')
#    file = File.open("data/fn/data#{j}.txt", 'wb+')
    for i in 0..(n-1)
        pos_curr = particle[i][1]
        pos_pbest = particle[i][2]
#    puts "\n"
#    puts "pos_gbest:#{pos_gbest}"
#    puts "pos_pbest:#{pos_pbest}"

	curr_v = particle[i][0]
#if (i == 1)
#   p "1st pos_curr:#{pos_curr}"
#   p "v[t]=#{curr_v}"
#end
       	next_pos[0] = pos_curr[0] + curr_v[0]
        next_pos[1] = pos_curr[1] + curr_v[1]
#if (i == 1)
#   p "after add v next_pos_:#{next_pos}"
#end

        # update v
        # v = c1*r1*(pos_pbest-pos_curr) + c2*r2*(pos_gbest-pos_curr)
	next_v = [0, 0] 
        next_v[0] = c1*r1*(pos_pbest[0]-pos_curr[0]) + c2*r2*(pos_gbest[0]-pos_curr[0])
        next_v[1] = c1*r1*(pos_pbest[1]-pos_curr[1]) + c2*r2*(pos_gbest[1]-pos_curr[1])
	particle[i][0] = next_v
#    p "pos_gbest=#{pos_gbest},pos_pbest=#{pos_pbest},pos_curr=#{pos_curr}"
#if (i == 1)
#    p "v[t+1]=#{next_v}"
#end
#    p "v: #{v}"

        # update pos_curr
#        if ( (particle[i][1][0]+v[0]).abs < max*2 )
#            if ( (particle[i][1][1]+v[1]).abs < max*2 )
#               end
#        end
#if (i == 1)
#    p "pos_curr:#{pos_curr}"
#    p "particle[i][1]=#{particle[i][1]}"
#    p "z=:#{particle[i][3]}"
#    p "orgin fn(z)=#{fn(particle[i][1][0], particle[i][1][1])}"
#    p "new fn(z)=#{fn(next_pos[0], next_pos[1])}"
#    gets
#end
        # update pos_pbest
        z = fn(next_pos[0], next_pos[1])
        if (z < particle[i][3])
	    particle[i][1][0] = next_pos[0]
	    particle[i][1][1] = next_pos[1]
            particle[i][2] = particle[i][1]
            particle[i][3] = z
        end
        # update gbest
#       if (z < z_gbest)
#            z_gbest = z
#            tmp_gbest = pos_curr 
#        end

	if ( j == t )
		#p "t=#{j} z=#{particle[i][3]}"
		z_sum = z_sum + particle[i][3]
	end
    
#        file << "#{particle[i][1][0]} #{particle[i][1][1]}\n"

    end
#    file.close
    #pos_gbest = tmp_gbest
end

mean_z_data[k] = z_sum / n
end
puts mean_z_data
# // Caculating for variance and standard-deviation. //

#y_sum = 0
#z_mean = 0
#j = 0
#arr = Array.new
#or i in 0..(runs-1)
#        if (mean_z_data[i] != 0)
#                arr[j] = mean_z_data[i]
#                j = j + 1
#        end
#end
#p arr
group_mean =  mean_z_data.inject{ |sum, el| sum + el }.to_f / mean_z_data.size
p "group_mean=#{group_mean}"
#y_mean = y_sum / 50
#p x_mean
#p y_mean
inter_dif = 0
#y_inter_dif = 0
for i in 0..(runs-1)
        inter_dif = inter_dif + ((mean_z_data[i] - group_mean) ** 2)
end
group_var = inter_dif / runs
p group_var
