all:
	go build mesh-generator.go

optmesh:
	pv ../meshfiles/100d-quad1.mesh \
		| ./mesher > ../meshfiles/100d-quad1.optimized.mesh

unoptmesh:
	./mesh-generator > ../meshfiles/100d-quad1.mesh
