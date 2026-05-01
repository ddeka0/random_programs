# Databases — A Comprehensive Report Outline (for Undergrad Course + Research Onramp)

> Structure: top-down — first **why** databases exist (problem space, data classes, use cases),
> then **what** kinds were built, then **how** they are built (architecture, storage, workflows),
> then **trade-offs**, then **research** of the last ~15 years, then **what is coming next**.

---

## 0. Brief Historical Motivation (one short page, sets context)

- Pre-database world: punched cards, magnetic tape, flat files, application-specific binary formats.
- Concrete pains that pushed the field forward:
  - Re-implementing the same I/O, indexing, and crash-recovery code in every application.
  - No way to share the same data across multiple programs reliably.
  - No declarative way to ask questions of data — everything was procedural.
  - No correctness guarantees when many users hit the same data simultaneously.
- Generational inflection points (named, dated):
  - Hierarchical and Network DBs (IMS, CODASYL) — 1960s.
  - Relational model and SQL (Codd 1970, System R, Ingres) — 1970s.
  - Client-server RDBMS era (Oracle, DB2, Sybase, SQL Server, PostgreSQL, MySQL) — 1980s–90s.
  - Internet scale & "NoSQL" (Google BigTable, Amazon Dynamo, Hadoop, MongoDB, Cassandra) — 2000s.
  - Cloud-native, NewSQL, lakehouse, vector, AI-native — 2010s–2020s.
- Why this matters for a course: every modern design choice is a reaction to a prior pain point.

---

## 1. The Problem Space — What Were People Trying to Solve?

### 1.1 Classes of Data Storage Requirements

- **Persistence / durability** — survive process crash, OS crash, power loss, disk failure.
- **Volume / scale** — bytes to exabytes; single node to globally distributed.
- **Concurrency** — many readers and writers simultaneously without corrupting each other's view.
- **Consistency / correctness** — no lost updates, no torn writes, defined isolation guarantees.
- **Latency** — sub-millisecond point lookups vs. multi-second analytical scans.
- **Throughput** — ops/sec for writes, queries/sec for reads, MB/s for scans.
- **Data integrity & constraints** — types, foreign keys, uniqueness, business rules.
- **Queryability** — beyond key lookup: filters, joins, aggregations, search, traversal, similarity.
- **Schema evolution & flexibility** — add fields without rewriting everything.
- **Availability** — uptime under partial failure (node down, network partition, AZ down).
- **Disaster recovery** — backups, point-in-time recovery, cross-region replication.
- **Security & access control** — authn, authz, row/column security, encryption at rest and in transit, auditing.
- **Multi-tenancy** — many customers safely sharing one system.
- **Geographic distribution** — data near users, data residency / sovereignty laws.
- **Compliance & auditability** — GDPR, HIPAA, SOX, immutable audit trails.
- **Cost efficiency** — $/GB, $/op, $/query; storage tiering; serverless.
- **Operability** — schema migrations, online DDL, observability, upgrades without downtime.
- **Interoperability** — standard wire protocols, drivers, BI tool support.

### 1.2 Classes of Data Themselves (what is being stored)

- **Structured / tabular** — well-defined schema, rows and columns.
- **Semi-structured** — JSON, XML, BSON, protobuf with optional fields.
- **Unstructured** — large blobs: images, video, PDFs, raw text.
- **Hierarchical / tree-shaped** — XML docs, file system metadata, org charts.
- **Graph / network** — social, fraud rings, knowledge graphs, dependencies.
- **Time-series** — ordered, append-heavy, value-per-timestamp (metrics, IoT, finance).
- **Spatial / geographic** — points, polygons, R-tree-friendly workloads.
- **Text / full-text** — natural language, tokenizable, ranked retrieval.
- **High-dimensional vectors / embeddings** — modern AI workloads.
- **Key-value pairs** — opaque values keyed by an identifier.
- **Streaming / event data** — unbounded, ordered, often replayable.
- **Logs / audit trails** — append-only, immutable.
- **Numerical / scientific** — arrays, matrices, tensors.
- **Wide / sparse** — millions of optional attributes per entity.
- **Multi-modal** — combinations of the above (e.g., image + text + metadata).

### 1.3 Use Cases / Access Patterns

- **OLTP** — high-volume short transactions; point reads and writes; strict consistency.
- **OLAP / analytics** — long-running scans, aggregations, joins over historical data.
- **HTAP** — both on the same data without ETL latency.
- **Search / full-text retrieval** — ranked text matching, faceting.
- **Recommendation / similarity** — k-nearest-neighbor over embeddings.
- **Time-range and downsampling queries** — "last 5 minutes", rollups.
- **Graph traversal** — k-hop, shortest path, pattern match, PageRank-like.
- **Stream processing / CEP** — pattern detection on moving data.
- **ETL / ELT / data movement** — bulk load, transformation pipelines.
- **Caching / session / ephemeral state** — sub-ms reads, TTL-based eviction.
- **Configuration / metadata / service discovery** — small, highly available, strongly consistent.
- **Counters, leaderboards, rate limiters** — high write contention on hot keys.
- **Geospatial queries** — "things within X km".
- **Audit / compliance reads** — append-only, tamper-evident.
- **Bulk batch processing** — nightly jobs, training set generation.
- **Event sourcing** — state derived by replaying an immutable log.
- **Real-time dashboards** — fresh aggregates with seconds-level lag.

---

## 2. A Taxonomy of Database Systems — The Categories That Emerged

### 2.1 The Axes Used to Categorize

- **Data model** — relational, document, key-value, wide-column, graph, time-series, vector, search, ledger, spatial, multi-model.
- **Storage layout** — row, column, hybrid (PAX), document tree, adjacency list, inverted index, LSM vs B-tree.
- **Consistency model** — strong, linearizable, sequential, causal, eventual.
- **Distribution model** — single-node, leader-follower, multi-leader, leaderless, sharded, geo-replicated.
- **Workload optimization** — OLTP, OLAP, HTAP, streaming.
- **Schema flexibility** — schema-on-write vs schema-on-read.
- **Deployment model** — embedded, on-prem, managed, serverless, edge.

### 2.2 Top-Level Categories (each will be deep-dived in §3–§7)

- **Relational / SQL (RDBMS)**
- **Key-Value stores**
- **Document stores**
- **Wide-column / column-family stores**
- **Graph databases**
- **Time-series databases**
- **Search engines**
- **In-memory / cache-oriented stores**
- **Vector databases**
- **Ledger / immutable / blockchain-inspired DBs**
- **Spatial / GIS databases**
- **NewSQL / distributed SQL**
- **HTAP systems**
- **Multi-model databases**
- **Streaming databases**
- **Embedded / lightweight DBs**
- **Data warehouses (columnar OLAP)**
- **Data lakes / lakehouses (open table formats)**
- **Object / object-relational DBs** (historical context)

---

## 3. Sub-Divisions Inside Each Category (with representative systems)

For each category list (a) sub-types, (b) example systems, (c) where they differ.

- **RDBMS**
  - Traditional disk-based row-stores (PostgreSQL, MySQL, Oracle, SQL Server, DB2).
  - In-memory RDBMS (HANA, MemSQL/SingleStore, VoltDB).
  - Embedded (SQLite, DuckDB-as-OLAP-embedded).
  - Distributed/NewSQL (CockroachDB, Spanner, YugabyteDB, TiDB).
- **Key-Value**
  - In-memory (Redis, Memcached).
  - Persistent embedded (RocksDB, LevelDB, LMDB).
  - Distributed (DynamoDB, Riak, Aerospike, FoundationDB).
- **Document**
  - JSON/BSON-native (MongoDB, Couchbase).
  - JSON inside RDBMS (PostgreSQL `jsonb`, MySQL JSON).
  - Edge/sync-first (PouchDB, RxDB).
- **Wide-Column**
  - BigTable lineage (HBase, Bigtable, Accumulo).
  - Dynamo + BigTable hybrid (Cassandra, ScyllaDB).
- **Graph**
  - Property graph (Neo4j, JanusGraph, TigerGraph, Memgraph, Neptune-PG).
  - RDF / triple stores (Stardog, GraphDB, Virtuoso, Neptune-RDF).
  - Graph-on-relational (Apache AGE, SQL/PGQ in PG17).
- **Time-Series**
  - Specialized engines (InfluxDB, VictoriaMetrics, QuestDB).
  - SQL extensions (TimescaleDB on PG).
  - Metric-focused (Prometheus, M3, Mimir).
  - Columnar TSDB (ClickHouse used for TS).
- **Search**
  - Inverted index engines (Elasticsearch, OpenSearch, Solr, Vespa, Meilisearch, Typesense).
  - Hybrid keyword + vector.
- **Vector**
  - Pure-vector (Pinecone, Milvus, Weaviate, Qdrant, Chroma).
  - Library-as-a-DB (FAISS, Annoy, ScaNN, DiskANN).
  - Vector-in-existing-DB (`pgvector`, Mongo Atlas Vector, Elastic, Redis).
- **In-memory / cache**
  - Redis, Memcached, KeyDB, Dragonfly.
- **Ledger / verifiable**
  - QLDB, ImmuDB, blockchain-style (BigchainDB).
- **Spatial**
  - PostGIS, MongoDB geo, Elasticsearch geo, dedicated systems.
- **HTAP**
  - SingleStore, TiDB+TiFlash, SAP HANA, Snowflake Unistore, Oracle Exadata.
- **Streaming DBs**
  - Materialize, RisingWave, ksqlDB, Apache Flink Table Store / Paimon.
- **Data warehouses**
  - Snowflake, BigQuery, Redshift, Databricks SQL, ClickHouse Cloud, Firebolt.
- **Lakehouses / open tables**
  - Delta Lake, Apache Iceberg, Apache Hudi.
- **Embedded analytical**
  - DuckDB, chDB.

---

## 4. Storage Layout — How Bytes Sit on Disk / in Memory

For each, explain *physical layout*, *page/block structure*, *what scans look like*, *what point lookups look like*.

- **Row-oriented (NSM)** — all columns of one row contiguous; great for OLTP point ops.
- **Column-oriented (DSM)** — each column stored separately; great for scans, compression, vectorization.
- **PAX / hybrid** — row groups internally column-organized (Parquet, ORC).
- **Heap files vs index-organized tables (clustered indexes).**
- **B+ tree-based engines** — InnoDB, PostgreSQL heap+btree, SQL Server.
- **LSM-tree-based engines** — RocksDB, Cassandra, HBase, ScyllaDB.
- **Hash-organized** — Redis dict, Memcached slab allocator.
- **Document tree on disk** — BSON, MongoDB WiredTiger.
- **Adjacency-list / index-free adjacency** — Neo4j; vs. matrix/CSR for analytical graph.
- **Inverted indexes** — postings lists, skip lists, doc-value columns; for search.
- **Time-partitioned chunks** — hypertables, shards by time bucket, retention policies.
- **Compressed columnar formats on object storage** — Parquet, ORC, Arrow IPC.
- **Memory-mapped storage** — LMDB; OS page cache as buffer pool.
- **Append-only logs** — Kafka segments, event sourcing logs.
- **Multi-version layouts (MVCC)** — old versions kept until vacuum/compaction.
- **Vector index layouts** — flat, IVF, HNSW graph, PQ-compressed, DiskANN.
- **Storage tiers & separation** — local SSD vs. object storage; "data lake on S3".
- **Encoding & compression** — dictionary, RLE, delta, frame-of-reference, bit-packing, Zstd.

---

## 5. Strengths — What Each Type Is Good At, and *Why* (Architectural Reason)

For every category in §2.2, write a short paragraph following this template:

- **What workload it dominates.**
- **Why** — point to (a) data layout, (b) access path / index, (c) concurrency model, (d) distribution model.
- **Concrete example query / operation** that shows the strength.

Examples (to be expanded):

- RDBMS row store excels at small, multi-row OLTP transactions because of B+ tree indexes, MVCC, and WAL-based recovery enabling fast point ops with ACID.
- Columnar warehouses dominate analytical scans because of column pruning, dictionary/RLE compression, and vectorized SIMD execution over packed columns.
- LSM stores dominate write-heavy workloads because all writes are sequential appends to a memtable + WAL, deferring random I/O to background compaction.
- Graph DBs dominate deep traversals because of index-free adjacency — neighbor lookup is a pointer hop, not a join.
- Inverted-index search engines dominate text retrieval because postings + scoring are designed around term-at-a-time / document-at-a-time evaluation.
- Vector DBs dominate similarity search because ANN indexes (HNSW/IVF) trade exactness for sublinear search in high dimensions.
- KV caches dominate sub-ms lookups because everything is in RAM behind a hash table.
- Time-series DBs dominate metric workloads because of time-bucketed partitioning, delta-of-delta and Gorilla compression, and append-only writes.

---

## 6. One or Two Critical Workflows per DB Type — and the CS Fundamentals They Touch

For each DB type, walk through 1–2 workflows end-to-end and label the underlying CS concepts.
The point is that the course should *teach the concept through the workflow*.

### 6.1 Concept Bank to Surface Across Workflows

- **Write-ahead logging (WAL)** & crash recovery (ARIES: analysis/redo/undo).
- **B+ tree** — search, insert, split, merge; height vs fanout vs cache lines.
- **LSM tree** — memtable, immutable SSTables, leveled vs tiered compaction; read/write/space amplification.
- **MVCC** — snapshot isolation, version chains, vacuum/GC.
- **Locking** — 2PL, deadlock detection, lock escalation, intent locks.
- **Optimistic concurrency control / OCC**.
- **Serializable Snapshot Isolation (SSI)**.
- **Query lifecycle** — parser → resolver → logical plan → optimizer → physical plan → executor.
- **Cost-based optimization** — statistics, histograms, cardinality estimation, dynamic programming for join order.
- **Join algorithms** — nested loop, sort-merge, hash join, broadcast vs shuffle.
- **Hashing** — open addressing, separate chaining, consistent hashing, rendezvous hashing.
- **Probabilistic structures** — Bloom, Cuckoo, Count-Min, HyperLogLog, Theta sketches.
- **Skip lists, tries, radix trees** (memtables, in-mem indexes).
- **Buffer pool / page cache management** — LRU, CLOCK, ARC, 2Q.
- **Sharding / partitioning** — hash, range, list, composite, consistent-hash rings.
- **Replication** — leader-follower, multi-leader, leaderless (Dynamo-style quorums).
- **Consensus** — Paxos, Multi-Paxos, Raft, Zab, EPaxos.
- **2PC / 3PC**, Saga pattern for distributed transactions.
- **Logical clocks** — Lamport, vector clocks, hybrid logical clocks (HLC), TrueTime.
- **CRDTs** for leaderless multi-master.
- **Anti-entropy** — Merkle trees, read repair, hinted handoff.
- **Inverted index construction** — tokenization, stemming, scoring (TF-IDF, BM25).
- **ANN algorithms** — HNSW (graph navigation), IVF (coarse + residual), PQ (product quantization), DiskANN.
- **Graph traversal** — BFS, DFS, Dijkstra, A*, label propagation.
- **Compression algorithms** — dictionary, RLE, delta, FOR, Snappy, Zstd, LZ4.
- **Vectorized execution & SIMD**.
- **Code generation / JIT** for queries.
- **Predicate / projection pushdown**, partition pruning.
- **Materialized views**, incremental view maintenance, dataflow / differential dataflow.
- **Backup mechanics** — physical vs logical, hot vs cold, PITR.
- **Network & RPC fundamentals** — connection pooling, protocol design, async I/O.
- **OS internals touched** — page cache, fsync, O_DIRECT, mmap, NUMA, scheduling.

### 6.2 Worked Workflow Examples (one or two per DB type)

- **RDBMS — `BEGIN ... COMMIT`** lifecycle:
Parse → plan → acquire row locks → write WAL record → buffer page dirty → commit fsync → checkpoint → recovery via redo/undo on crash.
Concepts: WAL, ARIES, durability, isolation levels, group commit, fsync.
- **RDBMS — `SELECT` with join**:
Parse → optimize (stats, join order DP) → choose hash vs merge → execute → return.
Concepts: cardinality estimation, cost models, join algorithms, buffer pool.
- **LSM KV — `PUT` then `GET`**:
PUT: WAL append + memtable insert; flush to SSTable; compaction.
GET: memtable → bloom-filtered SSTables top-down.
Concepts: write/read/space amplification, bloom filters, compaction policy.
- **Document DB — partial update**:
Document fetch → in-place update or copy-on-write → index maintenance for changed fields.
Concepts: storage of variable-length records, secondary index updates.
- **Wide-column — write to Cassandra**:
Coordinator hashes key → sends to N replicas → quorum (W) acks → eventual repair.
Concepts: consistent hashing, quorum (R+W>N), hinted handoff, read repair, gossip.
- **Graph — k-hop traversal**:
Start node → adjacency pointer chase → BFS frontier → filter by predicates.
Concepts: index-free adjacency, traversal algorithms, locality.
- **Search — query "best pizza near me"**:
Tokenize → look up postings → score with BM25 → merge with geo filter → top-K heap.
Concepts: inverted index, scoring functions, top-K, heap-based merging.
- **Vector — ANN query**:
Encode query → traverse HNSW graph or probe IVF lists → re-rank with PQ residuals.
Concepts: high-dimensional geometry, graph navigation, quantization.
- **Time-series — range aggregation**:
Partition pruning by time → columnar scan with delta-of-delta decode → SIMD aggregate.
Concepts: time partitioning, compression, vectorized aggregation.
- **Distributed SQL — cross-region txn**:
Optimistic prepare → Raft replicate per shard → 2PC with HLC/TrueTime ordering → commit wait.
Concepts: consensus, distributed transactions, clock uncertainty.
- **Warehouse — analytical scan on Parquet over S3**:
Metadata fetch → file/row-group pruning → column pull → vectorized exec → spill if needed.
Concepts: object-store I/O, columnar pruning, query parallelism, shuffle.
- **Streaming DB — incremental view maintenance**:
Source change → diff → propagate deltas through dataflow operators → update materialized result.
Concepts: dataflow, differential dataflow, watermarks, exactly-once semantics.
- **Cache — eviction under memory pressure**:
LRU/LFU pick victim → write-through or write-back → metrics.
Concepts: eviction policies, hit/miss ratio, working set.

---

## 7. Weaknesses — What Each Type Is Bad At, and *Why*

Same template as §5 but inverted. The "why" must always be tied to layout, access path, concurrency, or distribution.

Examples (to be expanded):

- RDBMS struggle with massive horizontal scale-out for writes due to single-leader coordination + 2PC costs.
- Pure columnar stores are bad at point updates because they must read/rewrite column files.
- LSM trees suffer read amplification + compaction-induced tail latency.
- Document DBs suffer for cross-document joins and complex multi-document transactions.
- Wide-column stores require careful query-driven schema design; ad-hoc analytics is painful.
- Graph DBs are bad at full-table aggregations (no columnar scan locality).
- Search engines are not transactional sources of truth; near-real-time indexing only.
- Vector DBs alone are weak at structured filters at scale (hence hybrid search).
- KV caches lose data on restart unless explicitly persisted; no rich queries.
- Time-series DBs are poor at random updates and joins outside time keys.
- Lakehouses suffer from small-file problems and metadata scaling.
- Distributed SQL pays latency cost for cross-shard transactions.
- Eventual-consistency systems surprise developers with stale/anomalous reads.

---

## 8. Cross-Cutting Foundations (worth a dedicated chapter)

These are theory + systems concepts that span all DB types. Treat as horizontal foundations.

- **ACID vs BASE**, and the spectrum in between.
- **Isolation levels** — RU, RC, RR, SI, Serializable; anomalies (dirty read, non-repeatable read, phantom, write skew, lost update).
- **CAP theorem**, **PACELC** (latency vs consistency even when no partition), end of "CAP-as-tagline".
- **Consistency models** — linearizable, sequential, causal, eventual, read-your-writes, monotonic.
- **Durability levels** — fsync per commit, group commit, async, quorum durability.
- **Indexing fundamentals** — primary, secondary, covering, partial, expression, multi-column, index-only scans.
- **Query optimization fundamentals** — rewrite rules, equivalence, statistics, adaptive plans.
- **Concurrency control families** — pessimistic (2PL), optimistic (OCC), MVCC, deterministic.
- **Replication topologies** — sync vs async, semi-sync, chain, geo.
- **Partitioning / sharding strategies** — hash, range, directory, rebalancing.
- **Backup & recovery models** — full/incremental/differential, PITR, logical vs physical.
- **Schema design & evolution** — normal forms vs denormalization; online schema change.
- **Security** — TLS, at-rest encryption, KMS integration, RBAC/ABAC, row/column security, audit.
- **Memory hierarchy awareness** — L1/L2/L3 caches → DRAM → PMEM → NVMe → SSD → HDD → object store.
- **OS interaction** — fsync, direct I/O, page cache, io_uring, NUMA placement.
- **Networking fundamentals** — RTTs, TCP, gRPC, RDMA, kernel-bypass.

---

## 9. ~15 Years of Database Research (≈2010–2025) — Bucketed

Goal: classify the research, not list every paper. For each bucket, name the problem,
the breakthrough idea, and 2–3 representative systems / papers.

### 9.1 Storage and Indexing

- **LSM-tree refinements** — leveled vs tiered, dynamic compaction, key-value separation (WiscKey, Titan).
- **Learned indexes** — RMI, ALEX, PGM; trade B-tree probe with model inference.
- **Adaptive / self-organizing indexes** — database cracking, adaptive merging.
- **Persistent memory storage engines** — log-structured PMEM, FAST + FAIR.
- **Disaggregated storage** — Aurora "log is the database", Socrates, PolarDB.
- **Modern open file formats** — Parquet, ORC, Arrow, Lance, Nimble.

### 9.2 Distributed Systems and Consensus

- **Raft** as a practical replacement for Paxos.
- **Spanner / TrueTime** — externally consistent, globally distributed SQL.
- **Calvin** — deterministic transactions to avoid distributed coordination.
- **EPaxos / Flexible Paxos** — leaderless / quorum-flexible consensus.
- **CRDTs** — formalization and adoption (Riak, Redis, Automerge).

### 9.3 Query Processing

- **Vectorized execution** — MonetDB/X100 → DuckDB, Velox, Photon.
- **JIT / compiled queries** — HyPer, Umbra, LLVM-based codegen.
- **Morsel-driven parallelism** — NUMA-aware, work-stealing executors.
- **Adaptive query processing** — re-plan mid-query.
- **Approximate query processing** — sampling, sketches, BlinkDB.
- **GPU-accelerated execution** — HeavyDB, BlazingSQL, RAPIDS.

### 9.4 NoSQL Era and Its Maturation

- Aftershocks of the **Dynamo paper** and **BigTable paper**.
- Convergence: NoSQL adding transactions and SQL; SQL adding JSON and flexibility.

### 9.5 NewSQL / Distributed SQL

- Spanner, F1, CockroachDB, TiDB, YugabyteDB, FaunaDB, FoundationDB-as-substrate.
- Themes: strong consistency at scale, online schema change, geo-partitioning.

### 9.6 HTAP — Hybrid Transactional/Analytical Processing

- Single-system designs: SAP HANA, SingleStore, TiDB+TiFlash, Oracle HeatWave.
- Snowflake **Unistore**, AlloyDB columnar engine, BigQuery + Spanner federation.

### 9.7 Cloud-Native and Serverless DBs

- **Separation of storage and compute** — Snowflake architecture, BigQuery, Redshift RA3.
- **Disaggregated logs** — Aurora's redo-log-shipping design.
- **Serverless DBs** — Aurora Serverless, Neon, PlanetScale, D1, Turso, CockroachDB Serverless.
- **Multi-tenant isolation** — noisy neighbor mitigation, fair scheduling.

### 9.8 Streaming & Real-Time

- **Log-as-substrate** — Kafka, Pulsar.
- **Stream processors** — Flink, Spark Structured Streaming, Beam.
- **Streaming databases** — Materialize, RisingWave, ksqlDB, ClickHouse with refreshable MVs.
- **CDC** — Debezium and the rise of "the database is just the log".

### 9.9 Vector / AI-Native Databases

- **Embedding-first retrieval** for LLM/RAG.
- **ANN algorithms** — HNSW (Malkov), IVF-PQ, ScaNN, DiskANN, SPANN.
- **Native vector DBs** — Pinecone, Milvus, Weaviate, Qdrant, Chroma, LanceDB.
- **Vector inside existing DBs** — `pgvector`, MongoDB Atlas Vector, Elastic, Redis, SQL Server.
- **Hybrid search** — sparse (BM25) + dense (vector) + filters.

### 9.10 Time-Series Innovations

- **Compression** — Gorilla (delta-of-delta + XOR-float), chunked columnar.
- **Retention + downsampling** as first-class features.
- **Metric-vs-event distinction** — Prometheus vs InfluxDB lineage.

### 9.11 Graph

- **Property graph standardization** — emerging GQL ISO standard (2024).
- **Graph-on-relational** — SQL/PGQ in PG17, Apache AGE.
- **Graph + analytics** — GraphBLAS, vertex-centric vs matrix-centric.
- **Knowledge graphs in the LLM era**.

### 9.12 Privacy and Security

- **Encrypted databases** — CryptDB, Always Encrypted, homomorphic experiments.
- **Differential privacy** — built into analytics layers.
- **Confidential computing / TEEs** — Intel SGX, AMD SEV-SNP, AWS Nitro Enclaves for DB workloads.
- **Privacy-preserving analytics** — secure multi-party computation, federated analytics.

### 9.13 Verifiable / Ledger Databases

- QLDB, ImmuDB, blockchain-DB hybrids; Merkle-backed audit trails.

### 9.14 Multi-Model Convergence

- ArangoDB, Cosmos DB, FaunaDB; PostgreSQL absorbing JSON, vector, time-series, geo.

### 9.15 Lakehouses and Open Table Formats

- Delta Lake, Iceberg, Hudi.
- ACID on object storage, time travel, schema evolution.
- Catalog wars — Unity, Polaris, Glue, Nessie.

### 9.16 Self-Driving / Autonomous DBs

- Auto-tuning, auto-indexing (Peloton/OtterTune lineage).
- ML for cardinality estimation (Naru, NeuroCard).
- ML for query optimization (Bao, Neo, Balsa).
- ML for buffer management and knob tuning.

### 9.17 Hardware-Driven Innovations

- **NVMe + io_uring** rewrites of storage layers.
- **RDMA + DPUs / SmartNICs** — kernel-bypass replication and shuffles.
- **GPU databases**, **FPGA acceleration**.
- **Persistent memory** designs (and the post-Optane retreat).
- **Computational / near-data storage**.

### 9.18 Benchmarking and Methodology

- TPC-C/H/DS critique; new benchmarks (TPCx-AI, ClickBench, JOB).
- Reproducibility movement (CIDR, SIGMOD reproducibility).

---

## 10. Modern Trends and What's Coming Next

Frame as: signal → driver → likely 3–5 year direction.

- **AI-native databases** — vectors as first-class, LLM-aware query planning, natural-language-to-SQL inside the engine.
- **Vector + relational unification** — most major SQL DBs ship a vector index by default.
- **Lakehouse maturity** — Iceberg / Delta / Hudi consolidate; warehouses read open formats directly.
- **Open table formats winning over proprietary storage** — multi-engine on shared tables.
- **Polyglot persistence ↔ multi-model convergence** — fewer specialized stores per app.
- **Serverless everywhere** — pay-per-query, scale-to-zero, branchable databases (Neon/PlanetScale style).
- **Edge databases** — SQLite / DuckDB / Turso / D1 at the edge with global sync.
- **Disaggregation taken further** — compute, storage, metadata, log all independent services.
- **Self-driving DBs** — ML-tuned knobs, auto-indexing, auto-partitioning becoming production-grade.
- **HTAP becoming default** — operational + analytical on the same data.
- **WebAssembly + DBs** — DuckDB-WASM, SQLite-WASM, in-browser analytics.
- **Real-time analytics on operational data** — incremental view maintenance everywhere.
- **Federated / data mesh / virtualized query** — query across catalogs without copy.
- **Sustainability / energy-aware databases** — joules per query as a metric.
- **Privacy-preserving analytics by default** — DP, TEEs, encrypted indexes.
- **Quantum and neuromorphic storage / search** — early research.
- **Agentic / autonomous data systems** — LLM agents driving schema, indexing, query rewriting.
- **Convergence of streaming and batch** — one engine, two latencies.

---

## 11. A Decision Framework — How to Choose a Database

- Workload characterization checklist: read/write ratio, payload size, query shape, latency SLO, durability SLO, consistency need, scale, growth, geo, compliance.
- CAP / PACELC trade-off table for the candidates.
- Cost models: $/GB stored, $/GB scanned, $/op, $/connection.
- Operational considerations: managed vs self-host, ecosystem, talent availability.
- Anti-patterns: using a graph DB for analytics, using KV for relational integrity, using a warehouse as a transactional store, etc.

---

## 12. Pedagogical Recommendations for an Undergraduate Course

- **Suggested topic order** (mirrors this outline; introduce concept, then a system that exemplifies it):
  1. Why DBs exist; data classes; use cases.
  2. Relational model + SQL.
  3. Storage & indexing (B+ tree, LSM, columnar).
  4. Query processing & optimization.
  5. Transactions, ACID, concurrency control, recovery.
  6. Distribution, replication, partitioning, consensus.
  7. NoSQL families and CAP/PACELC.
  8. Search, time-series, graph, vector.
  9. Warehouses, lakehouses, streaming.
  10. Modern trends and research onramp.
- **Hands-on labs** (cheap, open-source, instructive):
  - Build a tiny KV store with WAL + memtable + SSTable.
  - Implement B+ tree insert/split/delete.
  - Write a toy query optimizer over a small relational engine.
  - Implement Raft for a 3-node KV.
  - Build an inverted-index search over Wikipedia dump.
  - Build an HNSW vector index from scratch.
- **Open-source systems worth dissecting**:
SQLite, PostgreSQL, DuckDB, RocksDB, ClickHouse, Apache Cassandra, Redis, etcd, Materialize.
- **Foundational papers / reading list** (representative):
  - Codd 1970 — relational model.
  - System R / Ingres papers.
  - ARIES (Mohan et al.) — recovery.
  - Stonebraker "What goes around comes around".
  - Dynamo (Amazon, 2007), BigTable (Google, 2006), GFS, MapReduce.
  - Spanner (Google, 2012).
  - Raft (Ongaro & Ousterhout, 2014).
  - Aurora (Verbitski et al., 2017).
  - DDIA — Kleppmann, *Designing Data-Intensive Applications* (textbook).
  - Garcia-Molina, Ullman, Widom — *Database Systems: The Complete Book*.
  - Ramakrishnan & Gehrke — *Database Management Systems*.
  - HNSW (Malkov & Yashunin, 2018).
- **Capstone project ideas**:
  - Mini distributed SQL with Raft + range sharding.
  - HTAP demo: row store + columnar replica with logical replication.
  - RAG-grade vector + keyword hybrid search engine.
  - Streaming materialized view engine on top of Kafka/CDC.
- **Connections to other CS courses**:
  - **OS** — file systems, page cache, fsync, scheduling.
  - **Algorithms & data structures** — trees, hashing, heaps, graphs, probabilistic structures.
  - **Distributed systems** — consensus, replication, time, failure models.
  - **Networks** — RPC, RDMA, congestion, geo latency.
  - **PL & compilers** — query parsers, optimizers, codegen, JIT.
  - **Theory** — relational algebra, complexity of query evaluation, conjunctive queries.
  - **Security** — crypto for at-rest/in-transit, TEEs, DP.
  - **ML/AI** — learned components, vector retrieval for LLMs.

---

## 13. Research Directions Worth Pursuing (for the Professor)

- **AI × DB**: learned cardinality and optimization that are robust to drift; LLM-driven schema design and query rewriting; embedding-aware storage.
- **Hardware × DB**: CXL-attached memory, computational storage, DPUs, NVMe-oF, GPU ANN at scale.
- **Privacy × DB**: practical encrypted indexes, DP-by-default analytics, cross-org federated queries with provable guarantees.
- **Energy / sustainability**: joules/query as a first-class metric; carbon-aware query routing.
- **Verifiable & tamper-evident** systems for regulated industries.
- **Streaming + batch unification**: incremental view maintenance as the default execution model.
- **Open table formats**: catalog interoperability, multi-engine consistency, deletion vectors at scale.
- **Self-driving DBs at production grade**: stability, explainability, safety of ML-tuned knobs.
- **Edge + sync**: CRDT-based offline-first databases for mobile/IoT/edge.
- **Quantum & neuromorphic storage / search** — long-horizon exploration.
- **Benchmarking & reproducibility** — new benchmarks for HTAP, vector, streaming, AI workloads.

---

## 14. Glossary (one-liner each)

- ACID, BASE, CAP, PACELC.
- WAL, MVCC, OCC, 2PL, SSI.
- LSM, B+ tree, HNSW, IVF, PQ.
- Sharding, replication, quorum, consensus.
- OLTP, OLAP, HTAP, ETL/ELT, CDC.
- Row store, column store, PAX.
- Linearizability, serializability, snapshot isolation.
- Bloom filter, HyperLogLog, Count-Min.
- Materialized view, incremental view maintenance.
- Lakehouse, open table format, time travel.

---

## 15. Foundational References / Core Further Reading

These are the cross-cutting "must-haves" — the rest is in §16 (per DB type) and §17 (surveys).

### 15.1 Textbooks (most accessible first)

- Martin Kleppmann — *Designing Data-Intensive Applications* (O'Reilly, 2017). The single best modern, accessible book covering all of the above.
- Alex Petrov — *Database Internals* (O'Reilly, 2019). Storage engines + distributed systems, very approachable.
- Bailis, Hellerstein, Stonebraker (eds.) — *Readings in Database Systems*, 5th edition ("Red Book"). Free at [http://www.redbook.io/](http://www.redbook.io/).
- Garcia-Molina, Ullman, Widom — *Database Systems: The Complete Book* (Pearson). Classic, formal, course-friendly.
- Ramakrishnan & Gehrke — *Database Management Systems* (McGraw-Hill). Classic alternative.
- Stonebraker & Hellerstein (eds.) — *Readings in Database Systems*, 4th edition. Includes the original "What Goes Around Comes Around" chapter.

### 15.2 Free Online Courses / Lecture Series (excellent quality)

- CMU 15-445/645 — *Intro to Database Systems* (Andy Pavlo). Slides, projects, and full video lectures.
  - Site: [https://15445.courses.cs.cmu.edu/](https://15445.courses.cs.cmu.edu/)
  - Videos (Fall 2022 playlist): [https://www.youtube.com/playlist?list=PLSE8ODhjZXjaKScG3l0nuOiDTTqpfnWFf](https://www.youtube.com/playlist?list=PLSE8ODhjZXjaKScG3l0nuOiDTTqpfnWFf)
- CMU 15-721 — *Advanced Database Systems* (Andy Pavlo). Modern OLAP internals.
  - Site: [https://15721.courses.cs.cmu.edu/spring2023/](https://15721.courses.cs.cmu.edu/spring2023/)
- MIT 6.5830 / 6.830 — *Database Systems*. Reading list and projects: [http://db.lcs.mit.edu/6.5830/](http://db.lcs.mit.edu/6.5830/)
- Berkeley CS186 — *Introduction to Database Systems*: [https://cs186berkeley.net/](https://cs186berkeley.net/)

### 15.3 Foundational Papers (the canon)

- E. F. Codd — "A Relational Model of Data for Large Shared Data Banks", CACM 1970. Foundational. [https://dl.acm.org/doi/10.1145/362384.362685](https://dl.acm.org/doi/10.1145/362384.362685)
- Mohan et al. — "ARIES: A Transaction Recovery Method Supporting Fine-Granularity Locking and Partial Rollbacks Using Write-Ahead Logging", ACM TODS 1992. Crash recovery canon.
- Chang et al. — "Bigtable: A Distributed Storage System for Structured Data", OSDI 2006. [https://research.google.com/archive/bigtable-osdi06.pdf](https://research.google.com/archive/bigtable-osdi06.pdf)
- DeCandia et al. — "Dynamo: Amazon's Highly Available Key-value Store", SOSP 2007. [https://s3.amazonaws.com/AllThingsDistributed/sosp/amazon-dynamo-sosp2007.pdf](https://s3.amazonaws.com/AllThingsDistributed/sosp/amazon-dynamo-sosp2007.pdf)
- Corbett et al. — "Spanner: Google's Globally-Distributed Database", OSDI 2012. [https://www.usenix.org/system/files/conference/osdi12/osdi12-final-16.pdf](https://www.usenix.org/system/files/conference/osdi12/osdi12-final-16.pdf)
- Ongaro & Ousterhout — "In Search of an Understandable Consensus Algorithm" (Raft), USENIX ATC 2014. [https://web.stanford.edu/~ouster/cgi-bin/papers/raft-atc14.pdf](https://web.stanford.edu/~ouster/cgi-bin/papers/raft-atc14.pdf)
- Verbitski et al. — "Amazon Aurora: Design Considerations for High Throughput Cloud-Native Relational Databases", SIGMOD 2017. [https://pages.cs.wisc.edu/~yxy/cs764-f20/papers/aurora-sigmod-17.pdf](https://pages.cs.wisc.edu/~yxy/cs764-f20/papers/aurora-sigmod-17.pdf)
- Dageville et al. — "The Snowflake Elastic Data Warehouse", SIGMOD 2016. [https://info.snowflake.net/rs/252-RFO-227/images/Snowflake_SIGMOD.pdf](https://info.snowflake.net/rs/252-RFO-227/images/Snowflake_SIGMOD.pdf)
- Armbrust et al. — "Lakehouse: A New Generation of Open Platforms…", CIDR 2021. [https://cs.stanford.edu/~matei/papers/2021/cidr_lakehouse.pdf](https://cs.stanford.edu/~matei/papers/2021/cidr_lakehouse.pdf)
- Malkov & Yashunin — "Efficient and robust approximate nearest neighbor search using Hierarchical Navigable Small World graphs" (HNSW), IEEE TPAMI 2018. [https://arxiv.org/abs/1603.09320](https://arxiv.org/abs/1603.09320)
- Kraska et al. — "The Case for Learned Index Structures", SIGMOD 2018. [https://arxiv.org/abs/1712.01208](https://arxiv.org/abs/1712.01208)

### 15.4 Conferences to Track (for ongoing research)

- **Systems-leaning**: SIGMOD, VLDB (PVLDB), CIDR, ICDE.
- **Adjacent**: OSDI, SOSP, NSDI, EuroSys, ATC (for systems/distributed angle).
- **Theory**: PODS.

### 15.5 Blogs / Sites Worth Following

- Andy Pavlo's CMU Database Group — [https://db.cs.cmu.edu/](https://db.cs.cmu.edu/)
- Martin Kleppmann — [https://martin.kleppmann.com/](https://martin.kleppmann.com/)
- Murat Demirbas (distributed systems / DBs) — [http://muratbuffalo.blogspot.com/](http://muratbuffalo.blogspot.com/)
- The Morning Paper (archive) — [https://blog.acolyer.org/](https://blog.acolyer.org/)
- Markus Winand (SQL indexing) — [https://use-the-index-luke.com/](https://use-the-index-luke.com/) and [https://modern-sql.com/](https://modern-sql.com/)

---

## 16. Recommended Reading per Database Type (Accessible-First)

For each category, listed in roughly increasing depth: (1) gentle intro / docs, (2) practitioner book or
deep blog, (3) one foundational or recent peer-reviewed paper. Pick whichever level fits the reader.

### 16.1 Relational / SQL (RDBMS)

- Markus Winand — *Use The Index, Luke!* (free online). The most painless way to internalize how SQL indexes really work. [https://use-the-index-luke.com/](https://use-the-index-luke.com/)
- Markus Winand — *Modern SQL* (free online). Window functions, CTEs, JSON, modern features. [https://modern-sql.com/](https://modern-sql.com/)
- Hironobu Suzuki — *The Internals of PostgreSQL* (free online). Pictures, diagrams, very approachable. [http://www.interdb.jp/pg/](http://www.interdb.jp/pg/)
- Official PostgreSQL docs — [https://www.postgresql.org/docs/current/](https://www.postgresql.org/docs/current/)
- Paper: Hellerstein, Stonebraker, Hamilton — "Architecture of a Database System", Foundations & Trends 2007. The single best architectural overview of an RDBMS. [https://dl.acm.org/doi/10.1561/1900000002](https://dl.acm.org/doi/10.1561/1900000002)

### 16.2 Key-Value Stores

- Redis docs / "Redis University" — [https://redis.io/docs/latest/](https://redis.io/docs/latest/)
- Alex Petrov — *Database Internals*, Part I (storage engines) — explains B-tree and LSM-tree KV engines clearly.
- RocksDB wiki on GitHub — concise, technical. [https://github.com/facebook/rocksdb/wiki](https://github.com/facebook/rocksdb/wiki)
- Paper: DeCandia et al. — *Dynamo* (SOSP 2007). Origin of consistent hashing + quorum + Merkle trees + vector clocks in production KV stores. [https://s3.amazonaws.com/AllThingsDistributed/sosp/amazon-dynamo-sosp2007.pdf](https://s3.amazonaws.com/AllThingsDistributed/sosp/amazon-dynamo-sosp2007.pdf)

### 16.3 Document Stores

- MongoDB University (free courses) — [https://learn.mongodb.com/](https://learn.mongodb.com/)
- MongoDB Manual — [https://www.mongodb.com/docs/manual/](https://www.mongodb.com/docs/manual/)
- Couchbase developer docs — [https://docs.couchbase.com/](https://docs.couchbase.com/)
- Kleppmann's *DDIA* chapters 2 ("Data Models") and 3 ("Storage and Retrieval") cover document stores really well.

### 16.4 Wide-Column Stores

- Apache Cassandra docs — [https://cassandra.apache.org/doc/latest/](https://cassandra.apache.org/doc/latest/)
- ScyllaDB blog and university — deep, vendor-neutral-ish technical writing. [https://www.scylladb.com/learn/](https://www.scylladb.com/learn/)
- Paper: Chang et al. — *Bigtable* (OSDI 2006). The wide-column blueprint. [https://research.google.com/archive/bigtable-osdi06.pdf](https://research.google.com/archive/bigtable-osdi06.pdf)
- Paper: Lakshman & Malik — "Cassandra — A Decentralized Structured Storage System", LADIS 2009. [https://www.cs.cornell.edu/projects/ladis2009/papers/lakshman-ladis2009.pdf](https://www.cs.cornell.edu/projects/ladis2009/papers/lakshman-ladis2009.pdf)

### 16.5 Graph Databases

- Robinson, Webber, Eifrem — *Graph Databases* (O'Reilly, 2nd ed., 2015). Free PDF from Neo4j. [https://neo4j.com/graph-databases-book/](https://neo4j.com/graph-databases-book/)
- Neo4j Developer Guides — [https://neo4j.com/developer/](https://neo4j.com/developer/)
- TigerGraph "Graph Gurus" / Memgraph blog for property-graph perspectives.
- Survey: Angles & Gutierrez — "Survey of Graph Database Models", ACM Computing Surveys 2008. [https://renzoangles.net/files/gdm2012.pdf](https://renzoangles.net/files/gdm2012.pdf)

### 16.6 Time-Series Databases

- TimescaleDB blog — practitioner-friendly explainers. [https://www.timescale.com/blog](https://www.timescale.com/blog)
- Prometheus docs — *the* metrics-DB primer. [https://prometheus.io/docs/introduction/overview/](https://prometheus.io/docs/introduction/overview/)
- InfluxDB docs — [https://docs.influxdata.com/](https://docs.influxdata.com/)
- Paper: Pelkonen et al. — "Gorilla: A Fast, Scalable, In-Memory Time Series Database", VLDB 2015. The classic TS compression paper. [https://www.vldb.org/pvldb/vol8/p1816-teller.pdf](https://www.vldb.org/pvldb/vol8/p1816-teller.pdf)
- Survey: Jensen, Pedersen, Thomsen — "Time Series Management Systems: A Survey", IEEE TKDE 2017. [https://vbn.aau.dk/ws/files/263103785/TKDE2740932.pdf](https://vbn.aau.dk/ws/files/263103785/TKDE2740932.pdf)

### 16.7 Search Engines (full-text retrieval)

- Manning, Raghavan, Schütze — *Introduction to Information Retrieval* (Cambridge, free online). The textbook. [https://nlp.stanford.edu/IR-book/](https://nlp.stanford.edu/IR-book/)
- Elasticsearch official guides — [https://www.elastic.co/guide/index.html](https://www.elastic.co/guide/index.html)
- Lucene's docs and the *Lucene in Action* book.
- Robertson — "The Probabilistic Relevance Framework: BM25 and Beyond" — concise explanation of modern scoring.

### 16.8 In-Memory / Cache Stores

- Redis docs (above).
- Petrov *Database Internals* chapter on in-memory engines.
- Memcached wiki — [https://github.com/memcached/memcached/wiki](https://github.com/memcached/memcached/wiki)
- Aerospike "What's a Hybrid-Memory Architecture?" white papers.

### 16.9 Vector Databases (AI / similarity search)

- Pinecone Learning Center — best-in-class beginner content on embeddings, ANN, vector search. [https://www.pinecone.io/learn/](https://www.pinecone.io/learn/)
- Weaviate developer docs — [https://weaviate.io/developers/weaviate](https://weaviate.io/developers/weaviate)
- Qdrant articles — [https://qdrant.tech/articles/](https://qdrant.tech/articles/)
- Paper: Malkov & Yashunin — *HNSW* (IEEE TPAMI 2018). Foundational ANN graph index. [https://arxiv.org/abs/1603.09320](https://arxiv.org/abs/1603.09320)
- Survey: Pan, Wang, Li — "Survey of Vector Database Management Systems", The VLDB Journal 2024. [https://dbgroup.cs.tsinghua.edu.cn/ligl/papers/vldbj2024-vectordb.pdf](https://dbgroup.cs.tsinghua.edu.cn/ligl/papers/vldbj2024-vectordb.pdf)

### 16.10 Ledger / Verifiable / Blockchain-Inspired DBs

- AWS QLDB documentation — [https://docs.aws.amazon.com/qldb/](https://docs.aws.amazon.com/qldb/)
- ImmuDB docs — [https://docs.immudb.io/](https://docs.immudb.io/)
- Crosby et al. — "Efficient Data Structures for Tamper-Evident Logging", USENIX Security 2009 (classic Merkle-log paper). [https://www.usenix.org/legacy/event/sec09/tech/full_papers/crosby.pdf](https://www.usenix.org/legacy/event/sec09/tech/full_papers/crosby.pdf)

### 16.11 Spatial / GIS Databases

- PostGIS official docs — [https://postgis.net/documentation/](https://postgis.net/documentation/)
- Obe & Hsu — *PostGIS in Action* (Manning).
- Open Geospatial Consortium (OGC) Simple Features for SQL specification.

### 16.12 NewSQL / Distributed SQL

- CockroachDB design docs — [https://www.cockroachlabs.com/docs/stable/architecture/overview.html](https://www.cockroachlabs.com/docs/stable/architecture/overview.html)
- TiDB documentation — [https://docs.pingcap.com/tidb/stable](https://docs.pingcap.com/tidb/stable)
- YugabyteDB architecture — [https://docs.yugabyte.com/preview/architecture/](https://docs.yugabyte.com/preview/architecture/)
- Paper: Pavlo & Aslett — "What's Really New with NewSQL?", SIGMOD Record 2016. [https://db.cs.cmu.edu/papers/2016/pavlo-newsql-sigmodrec2016.pdf](https://db.cs.cmu.edu/papers/2016/pavlo-newsql-sigmodrec2016.pdf)
- Paper: Spanner OSDI 2012 (above) and "Spanner: Becoming a SQL System", SIGMOD 2017. [https://dl.acm.org/doi/10.1145/3035918.3056103](https://dl.acm.org/doi/10.1145/3035918.3056103)

### 16.13 HTAP (Hybrid Transactional/Analytical)

- TiDB HTAP blog and docs (TiFlash) — [https://docs.pingcap.com/tidb/stable/tiflash-overview](https://docs.pingcap.com/tidb/stable/tiflash-overview)
- SingleStore architecture overview.
- Paper: Özcan, Tian, Tözün — "Hybrid Transactional/Analytical Processing: A Survey", SIGMOD 2017 (tutorial paper). [https://dl.acm.org/doi/10.1145/3035918.3054784](https://dl.acm.org/doi/10.1145/3035918.3054784)

### 16.14 Multi-Model Databases

- ArangoDB documentation — [https://www.arangodb.com/docs/stable/](https://www.arangodb.com/docs/stable/)
- Azure Cosmos DB docs (multi-API multi-model design) — [https://learn.microsoft.com/azure/cosmos-db/](https://learn.microsoft.com/azure/cosmos-db/)
- FaunaDB docs.

### 16.15 Streaming Databases / Stream Processing

- Tyler Akidau — "Streaming 101" (free O'Reilly article). The clearest gentle intro. [https://www.oreilly.com/radar/the-world-beyond-batch-streaming-101/](https://www.oreilly.com/radar/the-world-beyond-batch-streaming-101/)
- Tyler Akidau — "Streaming 102". [https://www.oreilly.com/radar/the-world-beyond-batch-streaming-102/](https://www.oreilly.com/radar/the-world-beyond-batch-streaming-102/)
- Akidau, Chernyak, Lax — *Streaming Systems* (O'Reilly book). Builds on the above two.
- Materialize blog (incremental view maintenance done well) — [https://materialize.com/blog/](https://materialize.com/blog/)
- Paper: Akidau et al. — "The Dataflow Model", VLDB 2015. [https://research.google.com/pubs/archive/43864.pdf](https://research.google.com/pubs/archive/43864.pdf)
- Paper: McSherry, Murray, Isaacs — "Differential Dataflow", CIDR 2013. [https://www.cidrdb.org/cidr2013/Papers/CIDR13_Paper111.pdf](https://www.cidrdb.org/cidr2013/Papers/CIDR13_Paper111.pdf)

### 16.16 Embedded / Lightweight Databases

- SQLite documentation (especially "Architecture" + "Quirks") — [https://www.sqlite.org/docs.html](https://www.sqlite.org/docs.html)
- "How does SQLite work?" by Jamie Brandon — gentle internals walkthrough.
- DuckDB documentation — [https://duckdb.org/docs/](https://duckdb.org/docs/)
- Paper: Raasveldt & Mühleisen — "DuckDB: an Embeddable Analytical Database", SIGMOD 2019 (demo). [https://duckdb.org/pdf/SIGMOD2019-demo-duckdb.pdf](https://duckdb.org/pdf/SIGMOD2019-demo-duckdb.pdf)

### 16.17 Data Warehouses (columnar OLAP)

- Inmon vs Kimball — start with Kimball, *The Data Warehouse Toolkit* (3rd ed.).
- Snowflake docs — [https://docs.snowflake.com/](https://docs.snowflake.com/)
- BigQuery whitepapers — [https://cloud.google.com/bigquery/docs/introduction](https://cloud.google.com/bigquery/docs/introduction)
- Paper: Dageville et al. — "The Snowflake Elastic Data Warehouse", SIGMOD 2016 (above).
- Paper: Melnik et al. — "Dremel: Interactive Analysis of Web-Scale Datasets", VLDB 2010. [https://research.google/pubs/dremel-interactive-analysis-of-web-scale-datasets-2/](https://research.google/pubs/dremel-interactive-analysis-of-web-scale-datasets-2/)

### 16.18 Lakehouse / Open Table Formats

- Databricks blog "What is a Lakehouse?" — [https://www.databricks.com/glossary/data-lakehouse](https://www.databricks.com/glossary/data-lakehouse)
- Apache Iceberg documentation — [https://iceberg.apache.org/docs/latest/](https://iceberg.apache.org/docs/latest/)
- Apache Hudi documentation — [https://hudi.apache.org/docs/overview/](https://hudi.apache.org/docs/overview/)
- Delta Lake documentation — [https://docs.delta.io/latest/index.html](https://docs.delta.io/latest/index.html)
- Paper: Armbrust et al. — *Lakehouse* (CIDR 2021). [https://cs.stanford.edu/~matei/papers/2021/cidr_lakehouse.pdf](https://cs.stanford.edu/~matei/papers/2021/cidr_lakehouse.pdf)
- Paper: Armbrust et al. — "Delta Lake: High-Performance ACID Table Storage over Cloud Object Stores", VLDB 2020. [https://www.vldb.org/pvldb/vol13/p3411-armbrust.pdf](https://www.vldb.org/pvldb/vol13/p3411-armbrust.pdf)

---

## 17. Survey Papers (peer-reviewed, organized by topic)

These are the surveys and tutorial-style papers that, taken together, cover most of §1–§13.
All have been published in reputable venues (SIGMOD, VLDB, CIDR, IEEE TKDE, ACM Computing Surveys,
or *Foundations and Trends in Databases*).

### 17.1 The Big Picture / Field Retrospectives

- Stonebraker & Hellerstein — "What Goes Around Comes Around", in *Readings in Database Systems*, 4th ed., 2005. The original sweep through 35 years of data models.
- **Stonebraker & Pavlo — "What Goes Around Comes Around… And Around…", SIGMOD Record 2024.** The 20-year follow-up; covers MapReduce, KV, document, wide-column, search, array, vector, graph DBs and modern implementation trends. **Single most useful one-stop survey for this report.** [https://db.cs.cmu.edu/papers/2024/whatgoesaround-sigmodrec2024.pdf](https://db.cs.cmu.edu/papers/2024/whatgoesaround-sigmodrec2024.pdf)
- Hellerstein, Stonebraker, Hamilton — "Architecture of a Database System", Foundations & Trends in Databases, 2007. The architectural reference. [https://dl.acm.org/doi/10.1561/1900000002](https://dl.acm.org/doi/10.1561/1900000002)

### 17.2 Storage and Indexing

- Luo & Carey — "LSM-based Storage Techniques: A Survey", The VLDB Journal 2020. Definitive LSM-tree survey. [https://arxiv.org/pdf/1812.07527](https://arxiv.org/pdf/1812.07527) · [https://link.springer.com/article/10.1007/s00778-019-00555-y](https://link.springer.com/article/10.1007/s00778-019-00555-y)
- Graefe — "Modern B-Tree Techniques", Foundations & Trends in Databases, 2011. The B-tree counterpart.
- Idreos et al. — "The Periodic Table of Data Structures", IEEE Data Engineering Bulletin 2018. A unifying framework for storage data structures. [http://daslab.seas.harvard.edu/periodic-table-of-data-structures/](http://daslab.seas.harvard.edu/periodic-table-of-data-structures/)
- Ferragina & Vinciguerra — recent survey on learned index structures (TKDE 2020). Plus the foundational Kraska et al. SIGMOD 2018 paper.

### 17.3 Column Stores and Analytical Engines

- Abadi, Boncz, Harizopoulos, Idreos, Madden — "The Design and Implementation of Modern Column-Oriented Database Systems", Foundations & Trends in Databases, 2013. Definitive column-store survey covering MonetDB, VectorWise, C-Store. [https://stratos.seas.harvard.edu/publications/design-and-implementation-modern-column-oriented-database-systems](https://stratos.seas.harvard.edu/publications/design-and-implementation-modern-column-oriented-database-systems)

### 17.4 Query Processing and Optimization

- Chaudhuri — "An Overview of Query Optimization in Relational Systems", PODS 1998. The canonical optimizer survey.
- Graefe — "Query Evaluation Techniques for Large Databases", ACM Computing Surveys 1993. Foundational.
- Leis et al. — "How Good Are Query Optimizers, Really?", PVLDB 2015. Empirical reality check on optimizers. [https://www.vldb.org/pvldb/vol9/p204-leis.pdf](https://www.vldb.org/pvldb/vol9/p204-leis.pdf)
- Kemper & Neumann — "HyPer: A hybrid OLTP&OLAP main memory database system based on virtual memory snapshots", ICDE 2011. JIT-compiled queries reference.

### 17.5 Transactions and Concurrency Control

- Wu, Arulraj, Lin, Xian, Pavlo — "An Empirical Evaluation of In-Memory Multi-Version Concurrency Control", PVLDB 2017. The MVCC reality check. [https://db.cs.cmu.edu/papers/2017/p781-wu.pdf](https://db.cs.cmu.edu/papers/2017/p781-wu.pdf)
- Bernstein, Hadzilacos, Goodman — *Concurrency Control and Recovery in Database Systems* (1987, free PDF online). The textbook.
- Adya, Liskov, O'Neil — "Generalized Isolation Level Definitions", ICDE 2000. Modern isolation levels framing.

### 17.6 Distributed Systems / Replication / Consensus

- Brewer / Gilbert & Lynch — CAP theorem (Brewer 2000 keynote, Gilbert & Lynch SIGACT News 2002).
- Abadi — "Consistency Tradeoffs in Modern Distributed Database System Design: CAP is Only Part of the Story" (PACELC), IEEE Computer 2012. [https://www.cs.umd.edu/~abadi/papers/abadi-pacelc.pdf](https://www.cs.umd.edu/~abadi/papers/abadi-pacelc.pdf)
- Howard et al. — "Flexible Paxos: Quorum intersection revisited", OPODIS 2016.
- van Renesse & Altinbuken — "Paxos Made Moderately Complex", ACM Computing Surveys 2015.
- Wang et al. — "Survey of Consensus Protocols", arXiv 2022 (broad overview of Paxos/Raft/Zab/EPaxos and BFT variants).

### 17.7 NoSQL and Polyglot Persistence

- Cattell — "Scalable SQL and NoSQL Data Stores", SIGMOD Record 2010. Early NoSQL survey.
- Davoudian, Chen, Liu — "A Survey on NoSQL Stores", ACM Computing Surveys 2018. [https://dl.acm.org/doi/10.1145/3158661](https://dl.acm.org/doi/10.1145/3158661)

### 17.8 NewSQL and Distributed SQL

- Pavlo & Aslett — "What's Really New with NewSQL?", SIGMOD Record 2016. [https://db.cs.cmu.edu/papers/2016/pavlo-newsql-sigmodrec2016.pdf](https://db.cs.cmu.edu/papers/2016/pavlo-newsql-sigmodrec2016.pdf)

### 17.9 HTAP

- Özcan, Tian, Tözün — "Hybrid Transactional/Analytical Processing: A Survey", SIGMOD 2017. [https://dl.acm.org/doi/10.1145/3035918.3054784](https://dl.acm.org/doi/10.1145/3035918.3054784)
- Boncz et al. — multiple updates on snapshot-based HTAP designs in CIDR / VLDB.

### 17.10 Cloud-Native Databases

- Verbitski et al. — "Amazon Aurora", SIGMOD 2017 (above).
- Antonopoulos et al. — "Socrates: The New SQL Server in the Cloud", SIGMOD 2019. [https://www.microsoft.com/en-us/research/uploads/prod/2019/05/socrates.pdf](https://www.microsoft.com/en-us/research/uploads/prod/2019/05/socrates.pdf)
- Cao et al. — "PolarDB Serverless", SIGMOD 2021.
- Survey: Sakr et al. — "A Survey of Large Scale Data Management Approaches in Cloud Environments", IEEE Communications Surveys & Tutorials, 2011 (older but useful baseline).

### 17.11 Streaming Systems

- Akidau et al. — "The Dataflow Model: A Practical Approach to Balancing Correctness, Latency, and Cost in Massive-Scale, Unbounded, Out-of-Order Data Processing", PVLDB 2015. [https://research.google.com/pubs/archive/43864.pdf](https://research.google.com/pubs/archive/43864.pdf)
- Carbone et al. — "Apache Flink: Stream and Batch Processing in a Single Engine", IEEE Data Engineering Bulletin 2015.
- Begoli et al. — "One SQL to Rule Them All — an Efficient and Syntactically Idiomatic Approach to Management of Streams and Tables", SIGMOD 2019.
- Survey: Hirzel et al. — "A Catalog of Stream Processing Optimizations", ACM Computing Surveys 2014. [https://dl.acm.org/doi/10.1145/2528412](https://dl.acm.org/doi/10.1145/2528412)

### 17.12 Time-Series

- Jensen, Pedersen, Thomsen — "Time Series Management Systems: A Survey", IEEE TKDE 2017. [https://vbn.aau.dk/ws/files/263103785/TKDE2740932.pdf](https://vbn.aau.dk/ws/files/263103785/TKDE2740932.pdf)
- Bader, Kopp, Falkenthal — "Survey and Comparison of Open Source Time Series Databases", BTW 2017.

### 17.13 Graph Databases

- Angles & Gutierrez — "Survey of Graph Database Models", ACM Computing Surveys 2008. [https://renzoangles.net/files/gdm2012.pdf](https://renzoangles.net/files/gdm2012.pdf)
- Sahu et al. — "The Ubiquity of Large Graphs and Surprising Challenges of Graph Processing", PVLDB 2017 / VLDBJ 2020.
- Besta et al. — "Demystifying Graph Databases: Analysis and Taxonomy of Data Organization, System Designs, and Graph Queries", ACM Computing Surveys 2023. [https://arxiv.org/abs/1910.09017](https://arxiv.org/abs/1910.09017)

### 17.14 Vector / ANN / AI-Native Databases

- Pan, Wang, Li — "Survey of Vector Database Management Systems", The VLDB Journal 2024. **The current go-to.** [https://dbgroup.cs.tsinghua.edu.cn/ligl/papers/vldbj2024-vectordb.pdf](https://dbgroup.cs.tsinghua.edu.cn/ligl/papers/vldbj2024-vectordb.pdf)
- Wang, Xu, Yue, Mottin — "A Comprehensive Survey and Experimental Comparison of Graph-Based Approximate Nearest Neighbor Search", PVLDB 2021. [https://www.vldb.org/pvldb/vol14/p1964-wang.pdf](https://www.vldb.org/pvldb/vol14/p1964-wang.pdf)
- Li, Zhang, Sun, Wang et al. — "Approximate Nearest Neighbor Search on High Dimensional Data — Experiments, Analyses, and Improvement", IEEE TKDE 2020.
- Foundational: Malkov & Yashunin — HNSW (TPAMI 2018, above); Jégou, Douze, Schmid — "Product Quantization for Nearest Neighbor Search", IEEE TPAMI 2011.

### 17.15 Search / Information Retrieval

- Manning, Raghavan, Schütze — *Introduction to Information Retrieval*, Cambridge 2008 (textbook, free online).
- Zobel & Moffat — "Inverted Files for Text Search Engines", ACM Computing Surveys 2006. [https://dl.acm.org/doi/10.1145/1132956.1132959](https://dl.acm.org/doi/10.1145/1132956.1132959)

### 17.16 Self-Driving / ML for Databases

- Pavlo et al. — "Self-Driving Database Management Systems", CIDR 2017. [https://db.cs.cmu.edu/papers/2017/p42-pavlo-cidr17.pdf](https://db.cs.cmu.edu/papers/2017/p42-pavlo-cidr17.pdf)
- Marcus et al. — "Bao: Making Learned Query Optimization Practical", SIGMOD 2021. [https://dl.acm.org/doi/10.1145/3448016.3452838](https://dl.acm.org/doi/10.1145/3448016.3452838)
- Survey: Zhou et al. — "Database Meets Artificial Intelligence: A Survey", IEEE TKDE 2022.

### 17.17 Lakehouse / Open Table Formats

- Armbrust et al. — *Lakehouse* (CIDR 2021).
- Armbrust et al. — *Delta Lake* (PVLDB 2020).
- (Iceberg and Hudi have design docs; survey-quality papers are still emerging.)

### 17.18 Data Privacy and Confidential Databases

- Popa et al. — "CryptDB: Protecting Confidentiality with Encrypted Query Processing", SOSP 2011. [https://people.csail.mit.edu/nickolai/papers/popa-cryptdb.pdf](https://people.csail.mit.edu/nickolai/papers/popa-cryptdb.pdf)
- Priebe et al. — "EnclaveDB: A Secure Database Using SGX", IEEE S&P 2018.
- Survey: Yiu et al. — "Database Encryption Survey", various venues; plus Dwork's differential-privacy foundational papers.

### 17.19 Hardware × Database

- Kallman et al. — "H-Store: A High-Performance, Distributed Main Memory Transaction Processing System", PVLDB 2008. (in-memory).
- Färber et al. — "The SAP HANA Database — An Architecture Overview", IEEE Data Engineering Bulletin 2012.
- Survey: Boncz et al. — repeatedly on memory-hierarchy-aware query processing.
- "Modern Hardware for Future Databases" — multiple recent CIDR/VLDB tutorials on NVMe, RDMA, GPUs, CXL.

---

## 18. Technical Topic Reference Cards (use case · how it works · what only it solves · links)

For every major technical topic referenced in §1–§13, this section gives a 3-line "card":
**(1)** a concrete use case, **(2)** intuitive working internally, **(3)** the unique problem it
solves that other approaches cannot, and **(4)** one or two verified articles to learn more.
Each linked article was actually read before being included; quality and accessibility were checked.

### 18.1 Storage Engines & Indexes

**B+ tree** — *Use case*: PostgreSQL primary-key lookups, B-tree indexes in MySQL/InnoDB, point and range queries on OLTP tables. *How it works*: a balanced fan-out tree where internal nodes route searches and all data lives in leaf pages linked together; depth is typically 4–5 levels even for hundreds of millions of rows, so a lookup is 4–5 random I/Os. *Why uniquely useful*: it gives you both fast point lookups *and* ordered range scans on disk-resident data, which a hash index cannot do and an LSM tree pays compaction cost for.

- Markus Winand — *Use The Index, Luke!* "The Tree" — visual, beginner-friendly: [https://use-the-index-luke.com/sql/anatomy/the-tree](https://use-the-index-luke.com/sql/anatomy/the-tree)
- PostgreSQL docs — "B-Tree Indexes": [https://www.postgresql.org/docs/current/btree.html](https://www.postgresql.org/docs/current/btree.html)

**LSM tree (Log-Structured Merge tree)** — *Use case*: write-heavy workloads in RocksDB, Cassandra, ScyllaDB, HBase, LevelDB, Bigtable. *How it works*: writes go to an in-memory memtable + WAL; full memtables flush to immutable sorted files (SSTables) on disk; background compaction merges SSTables to reclaim space and bound read amplification. *Why uniquely useful*: every write is a sequential append (no random writes, no in-place updates), giving order-of-magnitude better write throughput than B-trees, at the cost of read amplification and compaction-induced tail latency.

- ScyllaDB glossary — "What is a Log Structured Merge Tree?": [https://www.scylladb.com/glossary/log-structured-merge-tree](https://www.scylladb.com/glossary/log-structured-merge-tree)
- ScyllaDB docs — "Compaction" (deep dive on STCS / LCS / TWCS): [https://docs.scylladb.com/manual/branch-2025.4/kb/compaction.html](https://docs.scylladb.com/manual/branch-2025.4/kb/compaction.html)

**Row-oriented (NSM) storage** — *Use case*: classic OLTP — fetch one customer record (all columns) by primary key. *How it works*: each tuple is laid out contiguously on a page, with a slotted-page header pointing at variable-length rows; one I/O brings the whole row into the buffer pool. *Why uniquely useful*: when the workload reads or writes most columns of a row at once, NSM minimizes I/O and update overhead — column stores would have to touch many separate column files for the same row.

- *DDIA*, ch. 3 ("Storage and Retrieval") — Kleppmann's row-vs-column comparison.
- Suzuki — *PostgreSQL Internals* §1: [https://www.interdb.jp/pg/pgsql01.html](https://www.interdb.jp/pg/pgsql01.html)

**Column-oriented (DSM) storage** — *Use case*: warehouse scans — `SELECT AVG(amount) FROM sales WHERE year=2024`. *How it works*: each column is stored as its own contiguous file/array; queries read only the columns they reference; values are highly compressible (dictionary, RLE, FOR) and well-suited to vectorized SIMD execution. *Why uniquely useful*: it cuts I/O proportionally to the number of unused columns and unlocks order-of-magnitude compression and CPU-throughput gains that row stores cannot match for analytical scans.

- Daniel Abadi blog — "Column-Stores vs. Row-Stores": [https://dbmsmusings.blogspot.com/2017/10/distributed-consistency-at-scale.html](https://dbmsmusings.blogspot.com/2017/10/distributed-consistency-at-scale.html) (background) and Abadi et al. survey (above, §17.3).
- DuckDB blog — "Vectorized Execution": [https://duckdb.org/2025/03/14/duckdb-execution.html](https://duckdb.org/2025/03/14/duckdb-execution.html) (companion: [https://systeminternals.dev/duckdb/vectorized-execution/](https://systeminternals.dev/duckdb/vectorized-execution/))

**PAX / hybrid layout (Parquet, ORC)** — *Use case*: storing analytical data on object storage (S3, GCS) in a way that engines like Spark, Trino, DuckDB can all read efficiently. *How it works*: data is split into row groups; *within* a row group, each column's values are stored contiguously and compressed; min/max footers per row group enable predicate pushdown so unused groups are skipped. *Why uniquely useful*: it gives column-store scan economics on cloud object storage while remaining a self-describing single file — pure column files would be hard to manage at scale, pure row files would prevent column pruning.

- Parquet documentation: [https://parquet.apache.org/docs/file-format/](https://parquet.apache.org/docs/file-format/)
- "How Parquet works" by Plural Sight / Wes McKinney's "In Pursuit of the Open Format" talks (background reading).

**Heap file vs index-organized table** — *Use case*: PostgreSQL uses heap + separate indexes; MySQL InnoDB clusters table data inside the primary-key B-tree. *How it works*: in a heap, rows go anywhere a free slot exists and indexes hold (key → row-id) pointers; in an index-organized table the primary index *is* the table, so the leaf pages are the rows. *Why uniquely useful*: heaps make secondary indexes cheap and updates simple; clustered tables make primary-key lookups one I/O cheaper but make every secondary index a two-hop indirection.

- Suzuki — *PostgreSQL Internals* §1.3 (heap + TOAST).
- MySQL docs — "Clustered and Secondary Indexes": [https://dev.mysql.com/doc/refman/8.0/en/innodb-index-types.html](https://dev.mysql.com/doc/refman/8.0/en/innodb-index-types.html)

**Inverted index** — *Use case*: full-text search ("show docs containing 'distributed lock'") in Elasticsearch, OpenSearch, Solr, Lucene-based engines. *How it works*: documents are tokenized into terms; for every term the engine stores a sorted *postings list* of doc IDs (often with positions and term frequencies); a query intersects/unions postings and scores results with BM25. *Why uniquely useful*: it turns "search by content" into the same shape as a key lookup — a B-tree or hash index over rows would still need a full scan to find which documents contain a given word.

- Elastic — "Inverted Index" (definitive guide): [https://www.elastic.co/guide/en/elasticsearch/guide/current/inverted-index.html](https://www.elastic.co/guide/en/elasticsearch/guide/current/inverted-index.html)
- "How Apache Lucene Works": [https://www.abstractalgorithms.dev/how-lucene-works](https://www.abstractalgorithms.dev/how-lucene-works)

**HNSW (Hierarchical Navigable Small World)** — *Use case*: vector similarity search for RAG, recommendations, image/audio retrieval (Pinecone, Weaviate, Qdrant, pgvector all default to HNSW). *How it works*: build a multi-layer graph where the top layer has a few nodes with long-distance edges (highway), lower layers have more nodes with shorter edges (local roads); searches greedily descend layer by layer toward the query vector. *Why uniquely useful*: gives sub-linear (~O(log N)) approximate-nearest-neighbor search in high-dimensional space — exact KNN requires scanning all vectors, while tree indexes (KD-tree, ball tree) collapse to linear cost above ~20 dimensions.

- Pinecone Learn — "Hierarchical Navigable Small Worlds (HNSW)": [https://www.pinecone.io/learn/hnsw/](https://www.pinecone.io/learn/hnsw/)
- Luke Salamone — "How does HNSW work?" (concise companion piece): [https://lukesalamone.github.io/posts/how-does-hnsw-work/](https://lukesalamone.github.io/posts/how-does-hnsw-work/)

**IVF + Product Quantization (PQ)** — *Use case*: billion-scale vector search where memory is the bottleneck (FAISS, Milvus IVF-PQ indexes). *How it works*: IVF partitions vectors into ~√N Voronoi cells via k-means and at query time only probes a few nearest cells; PQ then compresses each vector by splitting it into sub-vectors and replacing each with the ID of its nearest centroid in a small codebook. *Why uniquely useful*: combined IVF-PQ shrinks memory ~32-97% while keeping ANN search fast — flat search uses too much RAM at scale, and HNSW alone is memory-hungry because it stores graph edges per vector.

- Pinecone Learn — "Product Quantization": [https://www.pinecone.io/learn/product-quantization/](https://www.pinecone.io/learn/product-quantization/)
- Pinecone Learn — "Introduction to FAISS": [https://www.pinecone.io/learn/faiss-tutorial/](https://www.pinecone.io/learn/faiss-tutorial/)

**Bloom filter** — *Use case*: skip a disk read when an SSTable definitely doesn't contain a key (Cassandra, RocksDB, BigTable); decide whether a URL is in the malicious-URL set (Chrome). *How it works*: a bit array of size *m* with *k* hash functions; insert sets *k* bits, query checks all *k* bits — if any is 0 the key is *definitely absent*; if all are 1 the key is *probably present*. *Why uniquely useful*: gives "definitely-not-in-set" answers in a few bytes per element with no false negatives, enabling LSM stores to avoid 90%+ of useless SSTable reads — a hash set would need >10× more memory for the same purpose.

- Bill Mill — "Bloom filters by example" (interactive, very gentle): [https://llimllib.github.io/bloomfilter-tutorial/](https://llimllib.github.io/bloomfilter-tutorial/)
- Baeldung CS — "Introduction to Bloom Filter": [https://www.baeldung.com/cs/bloom-filter](https://www.baeldung.com/cs/bloom-filter)

**HyperLogLog (HLL)** — *Use case*: "count distinct visitors per day" at Reddit/Redis/BigQuery scale, where exact counting would need too much memory. *How it works*: hash each item to a uniform bit string, look at the longest run of leading zeros seen; a longer run is exponentially rarer, so the maximum run length encodes log₂ of the cardinality; bucketing into 2^p registers and harmonic-mean-averaging gives a tight, low-variance estimate in only a few KB. *Why uniquely useful*: estimates billions-of-element cardinality with ~1–2% error in ~12 KB of memory, mergeable across shards — exact distinct-count needs O(n) memory, sampling is biased, MinHash answers a different question.

- Redis blog — "Counting unique items: HyperLogLog explained": [https://redis.io/blog/counting-unique-items-redis/](https://redis.io/blog/counting-unique-items-redis/) (companion: [https://aneeshdurg.me/posts/2024/03/19-understanding%20HyperLogLog](https://aneeshdurg.me/posts/2024/03/19-understanding%20HyperLogLog))

**Skip list** — *Use case*: Redis sorted sets (ZRANGE, leaderboards), in-memory memtables in LevelDB and RocksDB. *How it works*: a sorted linked list where each node is randomly promoted to additional "express" levels (with probability ½ per level), so search drops down levels until it finds the slot — a probabilistic balanced tree without rotations. *Why uniquely useful*: O(log n) search/insert/delete with simple lock-free or fine-grained-lock concurrent implementations, far easier to write correctly than a concurrent B-tree or red-black tree.

- Newvick — "How skip lists work and why databases use them": [https://newvick.com/posts/skip-lists/](https://newvick.com/posts/skip-lists/)
- Redis source `src/t_zset.c` plus the original Pugh 1990 paper for the academically curious.

**Merkle tree (hash tree)** — *Use case*: anti-entropy repair in Cassandra/Dynamo, content addressing in Git/IPFS, tamper-evident audit logs in QLDB/blockchain. *How it works*: leaves are hashes of data blocks; each internal node is the hash of its children's hashes; one root hash summarizes the entire dataset, and any block can be verified with O(log n) sibling hashes. *Why uniquely useful*: lets two replicas detect *which* sub-ranges differ in O(log n) network round trips rather than streaming the full dataset, and lets clients verify a block belongs to a dataset without trusting the server.

- Wikipedia — "Merkle tree" (clear, well-illustrated): [https://en.wikipedia.org/wiki/Merkle_tree](https://en.wikipedia.org/wiki/Merkle_tree)
- Cassandra docs — "Repair and Merkle trees": [https://cassandra.apache.org/doc/latest/cassandra/managing/operating/repair.html](https://cassandra.apache.org/doc/latest/cassandra/managing/operating/repair.html)

**Open table formats (Apache Iceberg, Delta Lake, Apache Hudi)** — *Use case*: turn a pile of Parquet files in S3 into a queryable, ACID-updatable table that Spark, Trino, Flink, Snowflake, and DuckDB can all read. *How it works*: the table is described by a metadata layer (a JSON manifest tree in Iceberg, a `_delta_log` JSON sequence in Delta, a timeline of commits in Hudi) that points at immutable Parquet/ORC data files; commits are atomic swaps of the metadata pointer, enabling time travel, schema evolution, and snapshot isolation on object storage. *Why uniquely useful*: bring database-grade ACID, partition evolution, and concurrent writes to cheap object storage — plain Parquet folders give you no atomicity or schema evolution, and proprietary warehouse formats lock you to one engine.

- Hudi blog — "Iceberg vs Delta Lake vs Hudi: A Comparative Look" (vendor-tinted but technically solid): [https://hudi.apache.org/blog/2024/10/07/iceberg-vs-delta-lake-vs-hudi-a-comparative-look-at-lakehouse-architectures](https://hudi.apache.org/blog/2024/10/07/iceberg-vs-delta-lake-vs-hudi-a-comparative-look-at-lakehouse-architectures)
- Original paper — Armbrust et al. "Lakehouse" CIDR 2021: [https://cs.stanford.edu/~matei/papers/2021/cidr_lakehouse.pdf](https://cs.stanford.edu/~matei/papers/2021/cidr_lakehouse.pdf)

---

### 18.2 Transactions & Concurrency Control

**ACID** — *Use case*: bank transfer that must move $100 from A to B even under concurrent transactions and process crashes. *How it works*: Atomicity via WAL/undo records (all-or-nothing), Consistency via constraint validation, Isolation via locks or MVCC, Durability via fsync of the commit log record. *Why uniquely useful*: ACID is the only model that gives an application developer a "single-machine illusion" — application code can ignore concurrency and crashes; weaker models force the application to handle anomalies it didn't even know existed.

- PostgreSQL docs — "Concurrency Control / Transactions": [https://www.postgresql.org/docs/current/mvcc-intro.html](https://www.postgresql.org/docs/current/mvcc-intro.html)
- *DDIA*, ch. 7 ("Transactions") — Kleppmann's gold-standard explanation.

**BASE (Basically Available, Soft state, Eventual consistency)** — *Use case*: shopping cart in DynamoDB, Cassandra-backed timelines, Riak counters. *How it works*: writes are accepted at any reachable replica; replicas converge in the background via gossip / read repair / anti-entropy; the application accepts that two reads can see different values briefly. *Why uniquely useful*: gives high availability under partitions and very low write latency for geographically distributed workloads — ACID systems must coordinate (and therefore block) on the network for the same operations.

- Werner Vogels — "Eventually Consistent" (CACM): [https://dl.acm.org/doi/10.1145/1466443.1466448](https://dl.acm.org/doi/10.1145/1466443.1466448) (free mirror often available).
- Kleppmann — "Please stop calling databases CP or AP": [https://martin.kleppmann.com/2015/05/11/please-stop-calling-databases-cp-or-ap.html](https://martin.kleppmann.com/2015/05/11/please-stop-calling-databases-cp-or-ap.html)

**SQL isolation levels (RU / RC / RR / SI / Serializable)** — *Use case*: choosing between PostgreSQL's `READ COMMITTED` (default, fast) and `SERIALIZABLE` (safest) for an inventory system. *How it works*: each level *forbids* a defined set of anomalies — Read Committed forbids dirty reads, Repeatable Read also forbids non-repeatable reads, Snapshot Isolation gives a consistent snapshot but allows write skew, Serializable forbids all anomalies. *Why uniquely useful*: weaker levels exist precisely because Serializable is expensive — picking the right level is how you trade correctness for throughput on a per-workload basis.

- CockroachDB blog — "What write skew looks like" (best explanation of why SI is not Serializable): [https://www.cockroachlabs.com/blog/what-write-skew-looks-like/](https://www.cockroachlabs.com/blog/what-write-skew-looks-like/)
- Jepsen — "Consistency models" hierarchy (with isolation levels): [https://jepsen.io/consistency](https://jepsen.io/consistency)

**MVCC (Multi-Version Concurrency Control)** — *Use case*: PostgreSQL, Oracle, MySQL InnoDB, CockroachDB — readers never block writers and vice versa. *How it works*: each row carries hidden `xmin`/`xmax` (creator/deleter transaction IDs); a transaction's snapshot is just a list of "active" XIDs at start time, and a row version is visible iff `xmin` committed before the snapshot and `xmax` did not. *Why uniquely useful*: long-running analytical reads can run alongside high-throughput writes without blocking — pure 2PL would force readers to either wait or take stale results, which is unacceptable at OLTP scale.

- Suzuki — "PostgreSQL MVCC" (with diagrams): [https://www.interdb.jp/pg/pgsql05.html](https://www.interdb.jp/pg/pgsql05.html)
- "PostgreSQL MVCC: A Hands-On Exploration" (xmin/xmax walked through with examples): [https://medium.com/@ductran999/postgresql-mvcc-a-hands-on-exploration-04c884ccb94f](https://medium.com/@ductran999/postgresql-mvcc-a-hands-on-exploration-04c884ccb94f)

**Two-Phase Locking (2PL)** — *Use case*: traditional textbook DB systems (DB2, SQL Server in the past) and pessimistic OLTP workloads where conflicts are frequent. *How it works*: every transaction has a *growing phase* where it acquires shared/exclusive locks on rows or pages, and a *shrinking phase* (after commit point) where it releases them; deadlocks are detected by a wait-for graph and resolved by aborting one transaction. *Why uniquely useful*: provides serializability with conceptual simplicity; for high-conflict workloads it can outperform OCC because it never wastes work on doomed transactions, but it serializes readers with writers.

- "Database Internals" by Alex Petrov — ch. 5 "Transaction Processing and Recovery" (cleanest modern textbook treatment).
- Marc Brooker — "Atomic Commitment: The Unscalability Protocol" (covers 2PL/2PC scaling): [https://brooker.co.za/blog/2022/10/04/commitment.html](https://brooker.co.za/blog/2022/10/04/commitment.html)

**Optimistic Concurrency Control (OCC)** — *Use case*: low-conflict workloads in FoundationDB, Spanner, etcd transactions, document DBs with conditional updates. *How it works*: a transaction reads freely without locks, accumulates a private write set, then at commit time validates that no concurrent writer has changed any key it read — if validation passes, writes are applied atomically; otherwise the transaction aborts and retries. *Why uniquely useful*: eliminates lock contention and lock-manager overhead in workloads where conflicts are rare; trades latency-under-contention (retries) for throughput-under-low-contention.

- FoundationDB docs — "Transactions and OCC": [https://apple.github.io/foundationdb/transaction-manifesto.html](https://apple.github.io/foundationdb/transaction-manifesto.html)
- Pavlo CMU 15-445 — "Optimistic Concurrency Control" lecture slides: [https://15445.courses.cs.cmu.edu/fall2023/slides/17-timestampordering.pdf](https://15445.courses.cs.cmu.edu/fall2023/slides/17-timestampordering.pdf)

**Snapshot Isolation (SI)** — *Use case*: default isolation level in Oracle, SQL Server `SNAPSHOT`, PostgreSQL `REPEATABLE READ`. *How it works*: every transaction reads from the consistent snapshot taken at its start time (via MVCC); writes succeed only if no other transaction wrote the same row between this transaction's start and commit ("first committer wins"). *Why uniquely useful*: gives readers a stable, repeatable view without blocking writers, with much higher throughput than Serializable — at the cost of *write skew* (two transactions can each read a constraint, both decide it's safe to update, and break it together).

- CockroachDB blog — "What write skew looks like": [https://www.cockroachlabs.com/blog/what-write-skew-looks-like/](https://www.cockroachlabs.com/blog/what-write-skew-looks-like/)
- Berenson et al. paper — "A Critique of ANSI SQL Isolation Levels" (the foundational paper): [https://www.microsoft.com/en-us/research/wp-content/uploads/2016/02/tr-95-51.pdf](https://www.microsoft.com/en-us/research/wp-content/uploads/2016/02/tr-95-51.pdf)

**Serializable Snapshot Isolation (SSI)** — *Use case*: PostgreSQL `SERIALIZABLE`, CockroachDB default, used when you want both Serializable correctness and MVCC throughput. *How it works*: run on top of SI but track read-write dependencies between concurrent transactions at runtime; abort one transaction if a "dangerous structure" of rw-anti-dependencies forms that could lead to a non-serializable schedule. *Why uniquely useful*: gives true Serializable guarantees with no read locks and no extra writes — pre-SSI, the only ways to get Serializable were 2PL (slow) or strict serial execution (single-threaded).

- CockroachDB blog — "Serializable, Lockless, Distributed: Isolation in CockroachDB": [https://www.cockroachlabs.com/blog/serializable-lockless-distributed-isolation-cockroachdb](https://www.cockroachlabs.com/blog/serializable-lockless-distributed-isolation-cockroachdb)
- Cahill, Röhm, Fekete — original SSI paper, SIGMOD 2008.

---

### 18.3 Recovery & Durability

**Write-Ahead Log (WAL)** — *Use case*: PostgreSQL `pg_wal/`, MySQL redo log, every serious database — survive a crash without losing committed transactions. *How it works*: before any data page is modified on disk, a log record describing the change is written and *fsync'd* to a sequential log file; commit means "log record reached durable storage". *Why uniquely useful*: turns expensive random page writes into cheap sequential log writes for the durability path, and gives crash recovery a single source of truth — without WAL you'd have to fsync every modified page on every commit.

- Suzuki — "WAL and Recovery" in *PostgreSQL Internals* §9: [https://www.interdb.jp/pg/pgsql09.html](https://www.interdb.jp/pg/pgsql09.html)
- *DDIA*, ch. 3 — Kleppmann's WAL explanation (paired with B-tree and LSM internals).

**ARIES recovery** — *Use case*: the recovery algorithm conceptually used by IBM DB2, SQL Server, and many modern systems with "no-force, steal" buffer policies. *How it works*: three passes after a crash — *Analysis* scans the log forward from the last checkpoint to rebuild the dirty-page table and active-transaction table; *Redo* re-applies all logged changes from the oldest dirty-page LSN forward (repeating history); *Undo* rolls back the still-active "loser" transactions in reverse log order. *Why uniquely useful*: supports stealing dirty pages to disk before commit and not forcing pages on commit — both essential for performance — while still guaranteeing atomic, durable recovery; simpler schemes (force/no-steal) are slower.

- Yash Agarwal — "Database Recovery Demystified: Understanding ARIES from First Principles": [https://yashagw.github.io/blog/db-recovery/](https://yashagw.github.io/blog/db-recovery/)
- Mohan et al. — original ARIES paper, ACM TODS 1992 (canon).

**Two-Phase Commit (2PC)** — *Use case*: ensure atomicity of a transaction that touches multiple shards / services / databases (e.g., XA transactions, Spanner cross-shard writes). *How it works*: a coordinator asks all participants to "prepare" (force-write the change durably and promise to commit); only if every participant votes YES does the coordinator log "commit" and tell everyone to apply; any NO or timeout triggers rollback everywhere. *Why uniquely useful*: gives strict atomicity across independent systems; nothing weaker (e.g., compensating transactions) can guarantee the participants commit *together*. The well-known cost: if the coordinator dies between phases, participants are stuck holding locks ("blocking protocol").

- Marc Brooker — "Exploring TLA+ with two-phase commit" (with model-checking intuition): [https://brooker.co.za/blog/2013/01/20/two-phase.html](https://brooker.co.za/blog/2013/01/20/two-phase.html)
- Marc Brooker — "Atomic Commitment: The Unscalability Protocol" (why 2PC limits scale): [https://brooker.co.za/blog/2022/10/04/commitment.html](https://brooker.co.za/blog/2022/10/04/commitment.html)

**Saga pattern** — *Use case*: a microservices order flow that spans payment, inventory, shipping, and notifications, each in its own database. *How it works*: model the workflow as a sequence of *local* ACID transactions; each step has a *compensating action* that semantically undoes it (refund, restock); a choreography (events) or orchestration (coordinator) drives the sequence and triggers compensations on failure. *Why uniquely useful*: gives "eventually atomic" outcomes across services that cannot share a single transaction — 2PC across HTTP services is impractical at scale, and doing nothing leaves you with permanent inconsistencies.

- Chris Richardson — "Pattern: Saga" (the canonical write-up): [https://microservices.io/patterns/data/saga.html](https://microservices.io/patterns/data/saga.html)
- Microsoft Azure Architecture Center — "Saga design pattern": [https://learn.microsoft.com/en-us/azure/architecture/patterns/saga](https://learn.microsoft.com/en-us/azure/architecture/patterns/saga)

---

### 18.4 Distributed Systems Foundations

**CAP theorem** — *Use case*: explaining why you can't have a single-region strongly-consistent DB that stays fully writable during a network partition (e.g., split-brain decisions in MongoDB / etcd / ZooKeeper). *How it works*: in any distributed store, when a network partition (P) splits replicas, the system must pick — return possibly stale data on each side (Availability) or refuse writes on the minority side (Consistency); you cannot have both. *Why uniquely useful*: CAP is a *negative* result that quickly classifies systems' fault behavior under partition; without it, vendors would (and did) claim impossible guarantees.

- Wikipedia — "CAP theorem" (clear modern framing): [https://en.wikipedia.org/wiki/CAP_theorem](https://en.wikipedia.org/wiki/CAP_theorem)
- Kleppmann — "A Critique of the CAP Theorem": [https://arxiv.org/abs/1509.05393](https://arxiv.org/abs/1509.05393)

**PACELC** — *Use case*: comparing globally-replicated databases that *all* claim CAP "CP" but feel very different in practice (Spanner vs DynamoDB vs Cosmos DB). *How it works*: extends CAP — *if* there's a Partition, choose A or C; *Else* (the 99% case), choose between Latency and Consistency, because synchronous geo-replication adds tens to hundreds of milliseconds. *Why uniquely useful*: CAP only describes partition-time behavior; PACELC captures the *normal-operation* tradeoff that actually dominates user-perceived performance and design decisions.

- Daniel Abadi — original PACELC paper (IEEE Computer): [https://www.cs.umd.edu/~abadi/papers/abadi-pacelc.pdf](https://www.cs.umd.edu/~abadi/papers/abadi-pacelc.pdf)
- Wikipedia — "PACELC theorem": [https://en.wikipedia.org/wiki/PACELC_theorem](https://en.wikipedia.org/wiki/PACELC_theorem)

**Consistency models (linearizability, sequential, causal, eventual)** — *Use case*: choosing what guarantee a client app can rely on in Cosmos DB, Cassandra, etcd. *How it works*: each model defines which orderings of operations are *legal* — Linearizability says every op appears to happen instantaneously between its invocation and response, Sequential that all clients see the same order (no real-time tie), Causal that operations only respect happens-before, Eventual that replicas converge eventually with no ordering promise in between. *Why uniquely useful*: these labels let you reason about safety without knowing the implementation — telling you, e.g., "you can't safely do unique-username registration on an eventually-consistent store, period."

- Jepsen — "Consistency Models" hierarchy (the canonical map): [https://jepsen.io/consistency](https://jepsen.io/consistency)
- Aphyr (Kyle Kingsbury) — "Strong consistency models": [https://aphyr.com/posts/313-strong-consistency-models](https://aphyr.com/posts/313-strong-consistency-models)

**Paxos** — *Use case*: the underlying consensus algorithm in Google Chubby, Spanner replication, Microsoft Azure Storage; the "ground truth" of distributed agreement. *How it works*: a *proposer* asks a majority of *acceptors* to PROMISE a proposal number, then sends an ACCEPT with the value (or the latest previously-accepted value it learned); a value is *chosen* once a majority of acceptors have accepted it for the same proposal number. *Why uniquely useful*: the first asynchronous-network consensus algorithm proven correct under message loss, reordering, and minority failures — without it, distributed databases couldn't even agree what the next log entry should be.

- matklad — "Notes on Paxos" (clearer than most): [https://matklad.github.io/2020/11/01/notes-on-paxos.html](https://matklad.github.io/2020/11/01/notes-on-paxos.html)
- The Morning Paper — "Paxos Made Simple" walkthrough: [https://blog.acolyer.org/2015/03/04/paxos-made-simple/](https://blog.acolyer.org/2015/03/04/paxos-made-simple/)

**Raft** — *Use case*: replication in etcd, Consul, CockroachDB, TiKV, MongoDB, RethinkDB — basically every "modern" CP system. *How it works*: at any time exactly one server is *leader* (won an election with randomized timeouts); the leader serializes commands into a replicated log; an entry is *committed* once a majority has it on disk; a follower whose log is behind catches up before voting. *Why uniquely useful*: provides Paxos-equivalent safety with substantially less reasoning effort and a clearer leader-based design — making it the consensus algorithm of choice for new systems since 2014.

- The Secret Lives of Data — interactive Raft animation (best onboarding): [https://thesecretlivesofdata.com/raft/](https://thesecretlivesofdata.com/raft/)
- raft.github.io — official site with paper, talks, and implementation list: [https://raft.github.io/](https://raft.github.io/)

**Replication topologies (single-leader, multi-leader, leaderless)** — *Use case*: PostgreSQL streaming replication (single-leader), Active-Active databases (multi-leader), DynamoDB / Cassandra (leaderless). *How it works*: single-leader sends all writes through one node and ships a log to followers; multi-leader accepts writes anywhere and reconciles conflicts (LWW, CRDTs, application-defined); leaderless writes to W replicas in parallel and reads from R such that R+W>N for read-your-writes. *Why uniquely useful*: each topology trades different things — single-leader gives you simple linearizable writes but a write bottleneck; multi-leader gives geo write locality at the cost of conflict resolution; leaderless gives availability under partition at the cost of stale reads.

- *DDIA*, ch. 5 ("Replication") — single best treatment.
- Amazon — Dynamo paper (SOSP 2007), foundational for leaderless: [https://s3.amazonaws.com/AllThingsDistributed/sosp/amazon-dynamo-sosp2007.pdf](https://s3.amazonaws.com/AllThingsDistributed/sosp/amazon-dynamo-sosp2007.pdf)

**Quorum (R + W > N)** — *Use case*: tunable consistency in Cassandra, ScyllaDB, DynamoDB ("strong" vs "eventual" reads). *How it works*: with N replicas, each write must be acknowledged by W of them and each read must contact R of them; if R + W > N, every read intersects every write in at least one replica, so reads can return the freshest value (using version numbers) without a leader. *Why uniquely useful*: lets the same cluster offer both strong consistency (e.g., R=W=quorum) and high availability (e.g., R=W=1) per request, without architectural change — a leader-based system would need a redesign to do that.

- Cassandra docs — "Configuring data consistency": [https://cassandra.apache.org/doc/latest/cassandra/architecture/dynamo.html#tunable-consistency](https://cassandra.apache.org/doc/latest/cassandra/architecture/dynamo.html#tunable-consistency)
- *DDIA*, ch. 5 — quorum reads/writes section.

**Sharding / partitioning strategies (hash, range, directory)** — *Use case*: scale a single logical table across many machines (Vitess, Citus, MongoDB, BigTable). *How it works*: *hash* sharding hashes the primary key and modulos onto shards (uniform load, but range scans cross all shards); *range* sharding splits the keyspace into contiguous ranges (good range scans, hot-spot risk); *directory* sharding keeps an explicit lookup table from key → shard (flexibility at the cost of metadata management). *Why uniquely useful*: choosing the right sharding scheme determines whether your hot-key workload spreads or melts a single shard — no replication topology can fix a bad partition function.

- *DDIA*, ch. 6 ("Partitioning") — definitive.
- Vitess docs — "Sharding" (concrete production patterns): [https://vitess.io/docs/22.0/user-guides/configuration-basic/sharding/](https://vitess.io/docs/22.0/user-guides/configuration-basic/sharding/)

**Consistent hashing** — *Use case*: assign keys to nodes in DynamoDB, Cassandra, Memcached, Discord's chat clusters such that adding/removing a node moves only ~1/N of keys, not all of them. *How it works*: hash both nodes and keys onto a virtual ring (e.g., 0…2³²); each key is owned by the first node clockwise from its hash; *virtual nodes* (each physical node owns ~150 random ring positions) smooth load. *Why uniquely useful*: stops the "thundering-herd cache miss" of naïve `hash(k) % N` whenever N changes — the only practical way to add/remove cache or shard nodes online without remapping nearly every key.

- David Karger et al. — original "Consistent Hashing" paper (1997, MIT): [https://people.csail.mit.edu/karger/Papers/web.pdf](https://people.csail.mit.edu/karger/Papers/web.pdf)
- Wikipedia — "Consistent hashing" (with virtual nodes): [https://en.wikipedia.org/wiki/Consistent_hashing](https://en.wikipedia.org/wiki/Consistent_hashing)

**CRDTs (Conflict-free Replicated Data Types)** — *Use case*: collaborative editing (Figma, Linear, Notion), offline-first mobile sync, Riak/Redis distributed counters. *How it works*: each replica accepts local writes and ships either its full state (state-based CRDT) or operation deltas (op-based) to peers; the merge function is mathematically commutative, associative, and idempotent, so replicas converge regardless of message ordering or duplication. *Why uniquely useful*: replicas can keep accepting writes during partitions and still converge to a meaningful value when merged — last-write-wins loses concurrent updates, locks/2PC can't be acquired across a partition.

- vlcn.io — "A Gentle Introduction to CRDTs": [https://vlcn.io/blog/intro-to-crdts](https://vlcn.io/blog/intro-to-crdts)
- Bartosz Sypytkowski — "An introduction to state-based CRDTs" (deeper): [https://www.bartoszsypytkowski.com/the-state-of-a-state-based-crdts/](https://www.bartoszsypytkowski.com/the-state-of-a-state-based-crdts/)

**Lamport / vector clocks** — *Use case*: ordering events in distributed systems where physical clocks are unreliable; detecting concurrent versions in Dynamo, Riak, Git. *How it works*: a Lamport clock is a single integer incremented on every local event and on receipt set to `max(local, received)+1` — gives a total order consistent with happens-before; a vector clock keeps one counter per process and so can also *detect* concurrency (neither vector ≤ the other). *Why uniquely useful*: the only way to reason about causality without synchronized physical clocks; lets a leaderless DB tell "this update happened after that one" vs "these two are concurrent and need conflict resolution."

- Lamport — "Time, Clocks, and the Ordering of Events in a Distributed System" (CACM 1978, foundational): [https://lamport.azurewebsites.net/pubs/time-clocks.pdf](https://lamport.azurewebsites.net/pubs/time-clocks.pdf)
- Murat Demirbas — "Lamport's logical clocks" blog post: [http://muratbuffalo.blogspot.com/2014/05/lamports-logical-clocks.html](http://muratbuffalo.blogspot.com/2014/05/lamports-logical-clocks.html)

**Hybrid Logical Clocks (HLC)** — *Use case*: timestamping in CockroachDB, YugabyteDB, MongoDB 4.x — needs to be (a) close to wall time and (b) causally correct without atomic clocks. *How it works*: each timestamp is a tuple `(physical, logical-counter)`; on a local event take `max(prev, now())` for the physical part and bump the counter on ties; on a receive take the max of local, received, and `now()`. *Why uniquely useful*: combines the human-readability and approximate-wall-time of NTP with the causality guarantees of Lamport clocks — neither alone is enough for a distributed DB that wants both time-travel queries and snapshot consistency.

- Sandeep Kulkarni, Murat Demirbas et al. — original HLC paper: [https://cse.buffalo.edu/~demirbas/publications/hlc.pdf](https://cse.buffalo.edu/~demirbas/publications/hlc.pdf)
- CockroachDB — "Hybrid Logical Clock (HLC) Timestamps" glossary: [https://www.cockroachlabs.com/glossary/distributed-db/hybrid-logical-clock-hlc-timestamps](https://www.cockroachlabs.com/glossary/distributed-db/hybrid-logical-clock-hlc-timestamps)

**TrueTime (Spanner)** — *Use case*: globally-consistent transactions in Google Spanner across continents — every commit has a meaningful global timestamp without coordinating with a central server. *How it works*: every datacenter runs GPS receivers and atomic clocks; the TrueTime API exposes uncertainty as an *interval* `[earliest, latest]` (~6 ms wide); to commit, Spanner picks a timestamp `t` past `latest` and waits out the uncertainty window so that no later transaction can pick a smaller `t`. *Why uniquely useful*: gives external (real-time) consistency at planetary scale without distributed coordination on every commit — HLC is similar but cheaper and approximate; bare NTP can be tens of ms off and would be unsafe.

- Google Cloud docs — "TrueTime and external consistency": [https://cloud.google.com/spanner/docs/true-time-external-consistency](https://cloud.google.com/spanner/docs/true-time-external-consistency)
- CockroachDB — "Living without atomic clocks" (how HLC compensates): [https://www.cockroachlabs.com/blog/living-without-atomic-clocks/](https://www.cockroachlabs.com/blog/living-without-atomic-clocks/)

**Anti-entropy / read repair / Merkle-tree repair** — *Use case*: keep replicas in sync in Cassandra, DynamoDB, Riak even when individual writes were lost or hinted-handoff failed. *How it works*: *read repair* compares replicas' versions on every read and pushes the freshest to laggards; *anti-entropy* periodically compares Merkle-tree summaries of token ranges between replicas and streams only the differing data ranges; *hinted handoff* buffers writes destined for an unavailable replica and replays them when it returns. *Why uniquely useful*: lets a leaderless system recover from arbitrary subsets of dropped writes without manual intervention — leader-based systems push the same problem onto re-replication, but only after they've already declared the replica failed.

- Cassandra docs — "Repair" + "Anti-entropy": [https://cassandra.apache.org/doc/latest/cassandra/managing/operating/repair.html](https://cassandra.apache.org/doc/latest/cassandra/managing/operating/repair.html)
- Riak docs — "Active anti-entropy" (with Merkle tree details): [https://docs.riak.com/riak/kv/latest/learn/concepts/active-anti-entropy/](https://docs.riak.com/riak/kv/latest/learn/concepts/active-anti-entropy/)

---

### 18.5 Query Processing

**Cost-based query optimizer (CBO) & cardinality estimation** — *Use case*: PostgreSQL planner deciding between hash join, merge join, and nested loop for a 6-table analytical query. *How it works*: parse SQL → generate equivalent logical plans (rewrite rules) → enumerate physical plans (System-R-style join-order DP) → cost each using histograms / statistics on each table → pick the cheapest. *Why uniquely useful*: lets users write declarative SQL without worrying about access paths — the optimizer decides per-query; without it, every query would need a hand-tuned execution plan, and any data shift would invalidate it.

- Leis et al. — "How Good Are Query Optimizers, Really?" (PVLDB 2015) — empirical reality check: [https://www.vldb.org/pvldb/vol9/p204-leis.pdf](https://www.vldb.org/pvldb/vol9/p204-leis.pdf)
- Use The Index Luke — "The Optimizer" chapter (intuitive, beginner-friendly): [https://use-the-index-luke.com/sql/explain-plan](https://use-the-index-luke.com/sql/explain-plan)

**Join algorithms (hash, sort-merge, nested loop, broadcast vs shuffle)** — *Use case*: combining `orders` and `customers` on `customer_id` — three radically different physical strategies. *How it works*: *Nested loop* is best when one side is tiny or already indexed on the join key; *hash join* builds a hash table on the smaller side and probes from the larger (best for unsorted big-vs-small); *sort-merge* sorts both inputs once and then walks them in lock-step (best when inputs are already ordered). *Why uniquely useful*: which one wins is workload-dependent — nothing else in the engine will save you if the optimizer picks the wrong join algorithm at scale.

- Andy Pavlo CMU 15-445 — "Join Algorithms" lecture: [https://15445.courses.cs.cmu.edu/fall2023/notes/11-joins.pdf](https://15445.courses.cs.cmu.edu/fall2023/notes/11-joins.pdf)
- *DDIA*, ch. 10 ("Batch Processing") — broadcast vs shuffle joins for distributed engines.

**Vectorized execution** — *Use case*: DuckDB, ClickHouse, Snowflake, CockroachDB analytical mode — turn a 100M-row scan into a 50K-batch scan. *How it works*: instead of the classic Volcano `next()`-per-tuple iterator, operators receive *vectors* of ~1–4K values per call; tight loops over those vectors fit in L1/L2 cache and let the compiler emit SIMD instructions; selection vectors avoid copying filtered-out rows. *Why uniquely useful*: amortizes function-call and branch-mispredict overhead over thousands of values — gives 10–100× CPU speedup on analytical scans that no amount of B-tree tuning can match in a row-at-a-time engine.

- DuckDB blog — "Push-based execution / vectorized model": [https://duckdb.org/2025/03/14/duckdb-execution.html](https://duckdb.org/2025/03/14/duckdb-execution.html) (and [https://systeminternals.dev/duckdb/vectorized-execution/](https://systeminternals.dev/duckdb/vectorized-execution/))
- CockroachDB blog — "How we built a vectorized execution engine": [https://www.cockroachlabs.com/blog/how-we-built-a-vectorized-execution-engine](https://www.cockroachlabs.com/blog/how-we-built-a-vectorized-execution-engine)

**JIT / compiled queries (HyPer, Umbra, PostgreSQL JIT)** — *Use case*: TPC-H Q1-style heavy expression evaluation in HyPer, Umbra, SingleStore; PostgreSQL `jit_above_cost`. *How it works*: at planning time the engine emits LLVM IR (or wasm, or C++) specialized for the exact query — types, predicates, and column layouts inlined — and JIT-compiles to native code; the resulting "data-centric" code keeps tuples in CPU registers across operators rather than materializing intermediate rows. *Why uniquely useful*: removes interpreter overhead for arithmetic-heavy queries; together with vectorization, it's how modern in-memory engines reach near-hand-coded throughput on analytical SQL.

- Thomas Neumann — "Efficiently Compiling Efficient Query Plans for Modern Hardware" (HyPer, VLDB 2011): [https://www.vldb.org/pvldb/vol4/p539-neumann.pdf](https://www.vldb.org/pvldb/vol4/p539-neumann.pdf)
- PostgreSQL docs — "JIT (Just-in-Time Compilation)": [https://www.postgresql.org/docs/current/jit.html](https://www.postgresql.org/docs/current/jit.html)

**Materialized views** — *Use case*: a dashboard that needs `SELECT region, SUM(sales) GROUP BY region` to answer in 50 ms instead of 5 s — precompute it. *How it works*: the view's query is run once and its result is *stored* like a regular table; subsequent queries read from the stored result; the view goes stale as base tables change and must be refreshed (full or incremental). *Why uniquely useful*: trades storage and refresh cost for read latency — regular views re-execute every time, summary tables managed by hand are error-prone, and pre-aggregated cubes lock you into a fixed schema.

- PostgreSQL docs — "Materialized Views": [https://www.postgresql.org/docs/current/rules-materializedviews.html](https://www.postgresql.org/docs/current/rules-materializedviews.html)
- Codelit — "Materialized View Refresh — Eager, Lazy, Incremental, Concurrent": [https://codelit.io/blog/database-materialized-view-refresh](https://codelit.io/blog/database-materialized-view-refresh)

**Incremental View Maintenance (IVM) / differential dataflow** — *Use case*: Materialize, RisingWave, Flink Table Store, ClickHouse refreshable MVs — keep a complex SQL view continuously fresh under a stream of changes. *How it works*: model the query as a *dataflow graph* of operators; when a row is inserted/updated/deleted, propagate only the *delta* through the graph (a `+1` or `-1` to a count, a hash-table probe for a join) and write the delta to the materialized output. *Why uniquely useful*: makes "materialized view + streaming" a single primitive — full refresh on every change is too expensive at scale, hand-coded stream processors are brittle and don't compose like SQL.

- Materialize blog — "Life in differential dataflow": [https://materialize.com/blog/life-in-differential-dataflow](https://materialize.com/blog/life-in-differential-dataflow)
- McSherry, Murray, Isaacs — "Differential Dataflow" CIDR 2013 paper: [https://www.cidrdb.org/cidr2013/Papers/CIDR13_Paper111.pdf](https://www.cidrdb.org/cidr2013/Papers/CIDR13_Paper111.pdf)

---

### 18.6 Workload Patterns & Data Movement

**OLTP (Online Transaction Processing)** — *Use case*: e-commerce checkout, banking, ticket booking — many short transactions, high concurrency, low latency, strict ACID. *How it works*: row-store + B-tree indexes for fast point ops, MVCC for concurrency, WAL for durability; queries touch a handful of rows by key and complete in milliseconds. *Why uniquely useful*: optimized end-to-end for "tiny query, lots of them, must not lose data" — running OLTP on a columnar warehouse means every single-row update rewrites whole column files.

- *DDIA*, ch. 3 ("Storage and Retrieval") — OLTP vs OLAP framing.
- Microsoft Learn — "OLTP vs OLAP": [https://learn.microsoft.com/en-us/azure/architecture/data-guide/relational-data/online-transaction-processing](https://learn.microsoft.com/en-us/azure/architecture/data-guide/relational-data/online-transaction-processing)

**OLAP (Online Analytical Processing)** — *Use case*: monthly revenue reports, ad-hoc analytics on terabytes — few queries but each scans much of the data. *How it works*: column store, heavy compression, partition pruning, vectorized/JIT execution, often run on a separate warehouse cluster (Snowflake, BigQuery, Redshift, ClickHouse). *Why uniquely useful*: trades update flexibility and per-row latency for orders-of-magnitude scan throughput; running analytics on the OLTP database creates lock contention and 10-100× slower scans.

- Daniel Abadi et al. — "Modern Column-Oriented Database Systems" (the survey, §17.3 above).
- Snowflake docs — "Key Concepts": [https://docs.snowflake.com/en/user-guide/intro-key-concepts.html](https://docs.snowflake.com/en/user-guide/intro-key-concepts.html)

**HTAP (Hybrid Transactional/Analytical Processing)** — *Use case*: Shopify-style "show me real-time sales analytics on the same data the cart writes to" — without an ETL pipeline. *How it works*: keep a row store for OLTP and *replicate* to a column store within the same system (TiDB+TiFlash, SingleStore, SAP HANA, Snowflake Unistore); the query planner routes row-friendly queries to the row replica and scan-friendly queries to the columnar replica. *Why uniquely useful*: removes the "ETL latency wall" — pure OLTP scales but analytics are slow; a separate warehouse is fast but minutes-to-hours stale; HTAP gets both at a few-seconds replication lag.

- TiDB docs — "TiFlash overview" (the cleanest HTAP architecture write-up): [https://docs.pingcap.com/tidb/stable/tiflash-overview](https://docs.pingcap.com/tidb/stable/tiflash-overview)
- Özcan, Tian, Tözün — "Hybrid Transactional/Analytical Processing: A Survey" (SIGMOD 2017): [https://dl.acm.org/doi/10.1145/3035918.3054784](https://dl.acm.org/doi/10.1145/3035918.3054784)

**Change Data Capture (CDC)** — *Use case*: stream every change in PostgreSQL into Kafka → Snowflake / Elasticsearch / a search index, so downstream systems are always fresh. *How it works*: instead of polling tables, the CDC tool tails the database's transaction log (PostgreSQL logical replication, MySQL binlog, Oracle redo) and emits one event per row change with before/after images, in commit order. *Why uniquely useful*: log-based CDC is the only way to capture *every* change (including deletes) with low overhead and ordering guarantees — query-based "modified_at > X" polling misses deletes and creates load on the source.

- Confluent — "How Change Data Capture Works": [https://www.confluent.io/blog/how-change-data-capture-works-patterns-solutions-implementation/](https://www.confluent.io/blog/how-change-data-capture-works-patterns-solutions-implementation/)
- Debezium docs — "Architecture": [https://debezium.io/documentation/reference/stable/architecture.html](https://debezium.io/documentation/reference/stable/architecture.html)

**Event sourcing** — *Use case*: audit-heavy domains (banking, claims, ride-sharing) where you must reconstruct any past state and trust the history. *How it works*: instead of mutating state in place, append every change as an immutable event to an append-only log; current state is the *fold* of all events; new projections (read models) are built by replaying the log. *Why uniquely useful*: gives perfect auditability, time-travel debugging, and the ability to add new read models retroactively — a state-only database loses the "why" of every change as soon as the next update overwrites it.

- Martin Fowler — "Event Sourcing" (the canonical reference): [https://martinfowler.com/eaaDev/EventSourcing.html](https://martinfowler.com/eaaDev/EventSourcing.html)
- Jay Kreps (LinkedIn) — "The Log: What every software engineer should know about real-time data's unifying abstraction": [https://engineering.linkedin.com/distributed-systems/log-what-every-software-engineer-should-know-about-real-time-datas-unifying](https://engineering.linkedin.com/distributed-systems/log-what-every-software-engineer-should-know-about-real-time-datas-unifying)

---

### 18.7 Modern / Research Concepts

**Learned indexes** — *Use case*: replace a B-tree on a 1-billion-row sorted column with a tiny ML model that predicts position from key — used in research systems and increasingly in production read-mostly stores. *How it works*: model the cumulative distribution function (CDF) of keys with a *Recursive Model Index* (RMI) — a top-level model picks a sub-model, the sub-model predicts an approximate position, and a small local search corrects the residual error. *Why uniquely useful*: when the data distribution is regular, a learned index is faster *and* far smaller than a B-tree; B-trees ignore distribution and pay full O(log n) regardless.

- The Morning Paper — "The case for learned index structures, Part I" (intuitive walkthrough): [https://blog.acolyer.org/2018/01/08/the-case-for-learned-index-structures-part-i/](https://blog.acolyer.org/2018/01/08/the-case-for-learned-index-structures-part-i/)
- Kraska et al. — original SIGMOD 2018 paper: [https://arxiv.org/abs/1712.01208](https://arxiv.org/abs/1712.01208)

**Disaggregated storage ("log is the database", Aurora-style)** — *Use case*: AWS Aurora, Microsoft Socrates, Alibaba PolarDB — get cloud-scale durability without rewriting the SQL engine. *How it works*: the compute node only ships *redo log records* to a multi-tenant distributed storage service; the storage layer applies the log in the background to materialize pages, replicates 6-way across 3 AZs, and lets any compute node read pages on demand. *Why uniquely useful*: cuts network I/O ~30× compared to shipping full pages, makes failover near-instant (no log replay on the new compute node), and scales storage independently of CPU — classic shared-disk and shared-nothing designs cannot do all three.

- Werner Vogels — "Amazon Aurora ascendant: how we designed a cloud-native relational database": [https://www.allthingsdistributed.com/2019/03/amazon-aurora-design-cloud-native-relational-database.html](https://www.allthingsdistributed.com/2019/03/amazon-aurora-design-cloud-native-relational-database.html)
- Verbitski et al. — Aurora paper, SIGMOD 2017: [https://pages.cs.wisc.edu/~yxy/cs764-f20/papers/aurora-sigmod-17.pdf](https://pages.cs.wisc.edu/~yxy/cs764-f20/papers/aurora-sigmod-17.pdf)

**Self-driving / autonomous databases (OtterTune, ML-tuned knobs)** — *Use case*: tune PostgreSQL or MySQL's hundreds of configuration knobs (`shared_buffers`, `work_mem`, `max_wal_size`…) for *your* workload without a DBA. *How it works*: collect workload metrics + current knob settings + observed throughput/latency; use Bayesian optimization or Gaussian processes to predict which knob change improves the objective the most; iterate, transferring knowledge across similar workloads. *Why uniquely useful*: a human DBA can't keep up with hundreds of interdependent knobs across hundreds of databases; rule-based defaults leave 30-50% performance on the table that ML closes automatically.

- CMU Database Group — "OtterTune" project page: [https://db.cs.cmu.edu/projects/autotune/](https://db.cs.cmu.edu/projects/autotune/)
- Kishore Kumar — "OtterTune walkthrough" (paper digest): [https://akcube.github.io/blog/ottertune-automatic-database-management-system-tuning-through-large-scale-machine-learning/](https://akcube.github.io/blog/ottertune-automatic-database-management-system-tuning-through-large-scale-machine-learning/)

**Lakehouses & open table formats (Iceberg / Delta / Hudi)** — *Use case*: keep one canonical copy of analytical data in S3 / GCS, query it from Snowflake, Databricks, Trino, DuckDB without copying. *How it works*: see "Open table formats" card in §18.1 for the metadata-tree mechanics; the *lakehouse* pattern then layers BI/ML access, time travel, schema evolution, and ACID transactions on top. *Why uniquely useful*: collapses the historical "data lake (cheap, no ACID) + data warehouse (ACID, locked-in)" two-tier into one — neither tier alone gives both open formats and warehouse-grade reliability.

- Databricks — "What is a Lakehouse?": [https://www.databricks.com/glossary/data-lakehouse](https://www.databricks.com/glossary/data-lakehouse)
- Armbrust et al. — Lakehouse CIDR 2021: [https://cs.stanford.edu/~matei/papers/2021/cidr_lakehouse.pdf](https://cs.stanford.edu/~matei/papers/2021/cidr_lakehouse.pdf)

**The Log as substrate (Kafka, change streams, Materialize)** — *Use case*: every microservice subscribes to one source-of-truth log instead of every service polling every other service's database. *How it works*: each event (DB change, user action, IoT measurement) is written once to a partitioned, ordered, durable log (Kafka, Pulsar); consumers read at their own pace; downstream stores are materialized views over the log. *Why uniquely useful*: turns N×N point-to-point integrations into N producers + N consumers around one log; the log gives ordering, replayability, and decoupling that no request/response API can match.

- Jay Kreps — "The Log": [https://engineering.linkedin.com/distributed-systems/log-what-every-software-engineer-should-know-about-real-time-datas-unifying](https://engineering.linkedin.com/distributed-systems/log-what-every-software-engineer-should-know-about-real-time-datas-unifying)
- Confluent docs — "Kafka design": [https://kafka.apache.org/documentation/#design](https://kafka.apache.org/documentation/#design)

**Vector databases & RAG (Retrieval-Augmented Generation)** — *Use case*: an LLM chatbot answers questions over your private documents by retrieving the top-k similar passages and feeding them to the model. *How it works*: chunk and embed documents into vectors; store in an ANN index (HNSW / IVF-PQ); at query time embed the user's question, retrieve the nearest chunks, and pass them to the LLM as context — often combined with keyword (BM25) results in a "hybrid search". *Why uniquely useful*: gives an LLM access to information beyond its training cutoff and updates instantly when documents change — fine-tuning is expensive and not real-time; full-text search alone misses semantic similarity.

- Pinecone Learn — "Retrieval Augmented Generation (RAG)": [https://www.pinecone.io/learn/retrieval-augmented-generation/](https://www.pinecone.io/learn/retrieval-augmented-generation/)
- Pan, Wang, Li — "Survey of Vector Database Management Systems" (VLDBJ 2024, §17.14 above).

---

## 19. Suggested Order to Hand to the LLM (when expanding into the final document)

If the final document should be ~10–20 pages, expanding *every* bullet will overshoot. A reasonable
prioritization for the LLM expansion pass:

1. Expand §0–§3 fully (history, requirements, data classes, use cases, taxonomy) — 2–3 pages.
2. For §4–§7, pick 6–8 representative DB types and treat each in ~½ page using the layout/strength/weakness/workflow template — 3–4 pages.
3. Expand §8 (cross-cutting) to ~1 page.
4. Expand §9 in summary form (one paragraph per research bucket) — 2 pages.
5. Expand §10 (modern trends) — 1 page.
6. Keep §11–§13 brief — ~1 page combined.
7. Use §15–§17 as the bibliography rather than expanding into prose.

---

## Appendix A — Items Added on Top of Your Original List

For transparency, these were not in your original 9 bullets and were added because they recur in
real syllabi, real interviews, and real research agendas:

- §0 brief historical motivation (puts every choice in context).
- §1.1 explicit list of *requirements* (durability, latency, concurrency, integrity, security, multi-tenancy, geo, compliance, cost, operability, interoperability).
- §1.2 explicit *data classes* (tabular, semi-structured, unstructured, hierarchical, graph, time-series, spatial, text, vectors, streaming, logs, scientific, wide/sparse, multi-modal).
- §2.1 *axes* used for taxonomy (so the categories don't feel arbitrary).
- §2.2 expansion of categories with: vector, ledger/verifiable, NewSQL, HTAP, multi-model, streaming DB, embedded, warehouses, lakehouses.
- §4 added: PAX, MVCC layout, log-structured append-only, vector index layouts, storage tiers, encoding/compression.
- §6 *concept bank* across workflows (ARIES, MVCC, Raft, CRDTs, HLC/TrueTime, Bloom/HLL, BM25, HNSW, vectorized exec, JIT, codegen, predicate pushdown, IVM).
- §8 cross-cutting foundations chapter (ACID/BASE, CAP/PACELC, isolation levels, durability levels, indexing, concurrency families, replication, partitioning, backup, security, memory hierarchy, OS interaction, networking).
- §9 expanded research buckets to include: learned indexes, disaggregation, vectorized/JIT execution, NewSQL, HTAP, cloud-native serverless, streaming/CDC, vector/AI-native, time-series, graph, privacy/TEEs, verifiable/ledger, lakehouses + open table formats, self-driving DBs, hardware-driven innovations, benchmarking & reproducibility.
- §10 modern trends: AI-native DBs, vector+relational unification, edge DBs, WASM DBs, federated/data mesh, sustainability, agentic data systems.
- §11 decision framework (so students can *choose*, not just *describe*).
- §12 pedagogical plan: topic order, hands-on labs, open-source systems, reading list, capstones, links to other CS courses.
- §13 explicit research directions for the professor.
- §14 glossary, §15 concrete foundational references with verified links.
- §16 *recommended reading per database type* with accessible-first ordering (gentle intro → practitioner book/blog → one foundational paper) and verified URLs.
- §17 *survey papers* organized by topic, all from reputable venues (SIGMOD, VLDB / PVLDB, CIDR, IEEE TKDE, ACM Computing Surveys, *Foundations and Trends in Databases*), with verified URLs where freely available.
- §18 *technical topic reference cards* — for ~50 cross-cutting database concepts (B+ tree, LSM, MVCC, Raft, CRDTs, HNSW, CDC, etc.), each with three lines (concrete use case · intuitive internals · the unique problem it solves vs alternatives) plus 1–2 articles whose contents were actually fetched and read before being recommended.
- §19 a suggested LLM-expansion order so the final document stays at "few pages" rather than ballooning.

