# Databases — A Comprehensive Report Outline

> Structure: top-down — first **why** databases exist (problem space, data classes, use cases),
> then **what** kinds were built, then **how** they are built (architecture, storage, workflows),
> then **trade-offs**, then **research** of the last ~15 years, then **what is coming next**, and
> finally a **pedagogical and research onramp** for the professor.

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

## 15. References / Further Reading (placeholder list, fill at write time)

- Foundational papers (Codd, ARIES, System R, Dynamo, BigTable, GFS, Spanner, Raft, Aurora, HNSW, Snowflake, Calvin).
- Textbooks: Kleppmann *DDIA*, Garcia-Molina/Ullman/Widom, Ramakrishnan & Gehrke, Hellerstein/Stonebraker *Readings in Database Systems* ("Red Book").
- Survey papers: "What Goes Around Comes Around" (Stonebraker & Hellerstein) and its 2024 follow-up; vector DB surveys; HTAP surveys; learned-index surveys.
- Conferences to track: SIGMOD, VLDB, ICDE, CIDR, EuroSys, OSDI, NSDI.
- Blogs / talks: Andy Pavlo's CMU 15-445 / 15-721, Martin Kleppmann's writings, Joe Hellerstein's lectures.

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
- §14 glossary and §15 references — concrete starting points for the LLM expansion pass.
