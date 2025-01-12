load('//aios/storage/indexlib:defs.bzl', 'indexlib_cc_library')
package(default_visibility=['//aios/storage/indexlib:__subpackages__'])
indexlib_cc_library(
    name='table',
    srcs=[],
    hdrs=[],
    visibility=['//visibility:public'],
    deps=[':NormalTableFactory', ':reader']
)
indexlib_cc_library(
    name='reader',
    srcs=[],
    hdrs=[],
    visibility=['//visibility:public'],
    deps=[':NormalTabletSessionReader']
)
indexlib_cc_library(
    name='NormalTableFactory',
    visibility=['//aios/storage/indexlib/table:__subpackages__'],
    deps=[
        ':NormalDiskSegment', ':NormalMemSegment', ':NormalSchemaResolver',
        ':NormalTabletBuiltinIndex', ':NormalTabletExportLoader',
        ':NormalTabletExporter', ':NormalTabletLoader',
        ':NormalTabletSessionReader', ':NormalTabletWriter',
        '//aios/autil:NoCopyable', '//aios/future_lite',
        '//aios/storage/indexlib/base:Status',
        '//aios/storage/indexlib/base:Types',
        '//aios/storage/indexlib/config:TabletSchema',
        '//aios/storage/indexlib/document:DocumentBatch',
        '//aios/storage/indexlib/document/normal:NormalDocumentFactory',
        '//aios/storage/indexlib/framework:ITabletFactory',
        '//aios/storage/indexlib/framework/index_task:IIndexOperationCreator',
        '//aios/storage/indexlib/framework/index_task:IIndexTaskPlanCreator',
        '//aios/storage/indexlib/framework/index_task:IIndexTaskResourceCreator',
        '//aios/storage/indexlib/index:IIndexFactory',
        '//aios/storage/indexlib/index:interface',
        '//aios/storage/indexlib/table/common:CommonTabletValidator',
        '//aios/storage/indexlib/table/normal_table:NormalTabletValidator',
        '//aios/storage/indexlib/table/normal_table/index_task:NormalTableResourceCreator',
        '//aios/storage/indexlib/table/normal_table/index_task:NormalTableTaskOperationCreator',
        '//aios/storage/indexlib/table/normal_table/index_task:NormalTableTaskPlanCreator'
    ],
    alwayslink=True
)
indexlib_cc_library(
    name='normal_tablet_reader',
    srcs=['NormalTabletReader.cpp', 'NormalTabletSearcher.cpp'],
    hdrs=['NormalTabletReader.h', 'NormalTabletSearcher.h'],
    deps=[
        ':Common', ':DocRangePartitioner', ':NormalSchemaResolver',
        ':NormalTabletBuiltinIndex', ':NormalTabletInfoHolder',
        ':NormalTabletMeta', ':NormalTabletMetrics',
        ':SortedDocIdRangeSearcher', '//aios/alog',
        '//aios/storage/indexlib/framework:TabletReader',
        '//aios/storage/indexlib/framework/mem_reclaimer:EpochBasedMemReclaimer',
        '//aios/storage/indexlib/index/attribute:attr_helper',
        '//aios/storage/indexlib/index/deletionmap:factory',
        '//aios/storage/indexlib/index/inverted_index/builtin_index/date:DateIndexReader',
        '//aios/storage/indexlib/index/inverted_index/builtin_index/range:RangeIndexReader',
        '//aios/storage/indexlib/index/summary:reader',
        '//aios/storage/indexlib/table/common:SearchUtil'
    ]
)
indexlib_cc_library(
    name='NormalTabletSessionReader',
    deps=[
        ':normal_tablet_reader',
        '//aios/storage/indexlib/table/common:CommonTabletSessionReader'
    ]
)
indexlib_cc_library(name='NormalTabletInfoHolder', deps=[':NormalTabletInfo'])
indexlib_cc_library(
    name='DocRangePartitioner',
    deps=['//aios/autil:log', '//aios/storage/indexlib/base:constants']
)
indexlib_cc_library(
    name='NormalTabletInfo',
    deps=[
        ':NormalTabletMeta', ':SegmentSortDecisionMaker',
        '//aios/storage/indexlib/framework:TabletData',
        '//aios/storage/indexlib/index/deletionmap:reader'
    ]
)
indexlib_cc_library(
    name='Common',
    srcs=[],
    deps=[
        '//aios/storage/indexlib/index/attribute:constants',
        '//aios/storage/indexlib/index/deletionmap:constants',
        '//aios/storage/indexlib/index/operation_log:constants',
        '//aios/storage/indexlib/index/primary_key:constants',
        '//aios/storage/indexlib/index/summary:constants',
        '//aios/storage/indexlib/table/normal_table/virtual_attribute:Common'
    ]
)
indexlib_cc_library(
    name='SortedDocIdRangeSearcher',
    deps=[
        ':DimensionDescription', ':NormalTabletMeta',
        '//aios/storage/indexlib/index/attribute:reader'
    ]
)
indexlib_cc_library(
    name='NormalTabletMeta',
    deps=[
        '//aios/alog', '//aios/autil:NoCopyable', '//aios/autil:json',
        '//aios/storage/indexlib/base:NoExceptionWrapper',
        '//aios/storage/indexlib/base:Status',
        '//aios/storage/indexlib/base:constants',
        '//aios/storage/indexlib/config:SortDescription',
        '//aios/storage/indexlib/file_system'
    ]
)
indexlib_cc_library(
    name='NormalTabletLoader',
    deps=[
        ':NormalTabletModifier', ':NormalTabletPatcher',
        '//aios/storage/indexlib/base:MemoryQuotaController',
        '//aios/storage/indexlib/document/document_rewriter:DocumentInfoToAttributeRewriter',
        '//aios/storage/indexlib/index:IIndexFactory',
        '//aios/storage/indexlib/index:IIndexReader',
        '//aios/storage/indexlib/index/operation_log:reader',
        '//aios/storage/indexlib/table:builtin_define',
        '//aios/storage/indexlib/table/common:CommonTabletLoader',
        '//aios/storage/indexlib/table/normal_table/virtual_attribute:VirtualAttributeIndexFactory'
    ]
)
indexlib_cc_library(
    name='NormalTabletExportLoader',
    deps=[
        ':NormalTabletModifier', ':NormalTabletPatcher',
        '//aios/storage/indexlib/index/operation_log:reader',
        '//aios/storage/indexlib/table:builtin_define',
        '//aios/storage/indexlib/table/common:CommonTabletLoader'
    ]
)
indexlib_cc_library(
    name='NormalTabletWriter',
    deps=[
        ':NormalDocIdDispatcher', ':NormalDocumentRewriteChainCreator',
        ':NormalMemSegment', ':NormalTabletBuiltinIndex',
        ':NormalTabletInfoHolder', ':NormalTabletModifier',
        ':NormalTabletParallelBuilder',
        '//aios/storage/indexlib/index:IndexerParameter',
        '//aios/storage/indexlib/table/common:CommonTabletWriter'
    ]
)
indexlib_cc_library(
    name='NormalTabletModifier',
    deps=[
        ':Common', '//aios/storage/indexlib/base:Types',
        '//aios/storage/indexlib/config:IIndexConfig',
        '//aios/storage/indexlib/config:TabletSchema',
        '//aios/storage/indexlib/document:DocumentBatch',
        '//aios/storage/indexlib/framework:TabletData',
        '//aios/storage/indexlib/index/attribute:modifier',
        '//aios/storage/indexlib/index/deletionmap:modifier',
        '//aios/storage/indexlib/index/inverted_index:InplaceInvertedIndexModifier',
        '//aios/storage/indexlib/index/inverted_index:PatchInvertedIndexModifier',
        '//aios/storage/indexlib/index/operation_log:modifier',
        '//aios/storage/indexlib/index/primary_key:factory',
        '//aios/storage/indexlib/index/primary_key:reader'
    ]
)
indexlib_cc_library(
    name='NormalTabletBuiltinIndex',
    srcs=[],
    hdrs=[],
    deps=[
        '//aios/storage/indexlib/index/attribute:factory',
        '//aios/storage/indexlib/index/deletionmap:factory',
        '//aios/storage/indexlib/index/inverted_index:factory',
        '//aios/storage/indexlib/index/operation_log:factory',
        '//aios/storage/indexlib/index/primary_key:factory',
        '//aios/storage/indexlib/index/statistics_term:factory',
        '//aios/storage/indexlib/index/summary:factory'
    ]
)
indexlib_cc_library(
    name='NormalTabletPatcher',
    deps=[
        ':Common', ':NormalTabletModifier', '//aios/autil:log',
        '//aios/storage/indexlib/base:Status',
        '//aios/storage/indexlib/base:Types',
        '//aios/storage/indexlib/config:TabletSchema',
        '//aios/storage/indexlib/framework:Segment',
        '//aios/storage/indexlib/index/attribute:patch',
        '//aios/storage/indexlib/index/common/patch:PatchFileFinder',
        '//aios/storage/indexlib/index/deletionmap:modifier',
        '//aios/storage/indexlib/index/inverted_index:patch'
    ]
)
indexlib_cc_library(
    name='NormalMemSegment',
    deps=[
        ':NormalSchemaResolver', ':SegmentSortDecisionMaker',
        '//aios/storage/indexlib/index:DocMapDumpParams',
        '//aios/storage/indexlib/index/attribute:indexer',
        '//aios/storage/indexlib/index/common:SortValueConvertor',
        '//aios/storage/indexlib/table/plain:PlainMemSegment'
    ]
)
indexlib_cc_library(
    name='NormalDiskSegment',
    deps=[
        ':NormalSchemaResolver',
        '//aios/storage/indexlib/index/primary_key:constants',
        '//aios/storage/indexlib/table/plain:PlainDiskSegment'
    ]
)
indexlib_cc_library(
    name='NormalDocumentRewriteChainCreator',
    deps=[
        ':Common', '//aios/storage/indexlib/config:TabletSchema',
        '//aios/storage/indexlib/document/document_rewriter:DocumentInfoToAttributeRewriter',
        '//aios/storage/indexlib/document/document_rewriter:DocumentRewriteChain',
        '//aios/storage/indexlib/document/document_rewriter:TTLSetter',
        '//aios/storage/indexlib/table:builtin_define',
        '//aios/storage/indexlib/table/normal_table/virtual_attribute:Common',
        '//aios/storage/indexlib/table/normal_table/virtual_attribute:VirtualAttributeConfig'
    ]
)
indexlib_cc_library(
    name='NormalSchemaResolver',
    implementation_deps=['//aios/storage/indexlib/indexlib/config'],
    deps=[
        ':Common', '//aios/storage/indexlib/base:Status',
        '//aios/storage/indexlib/config:FileCompressConfigV2',
        '//aios/storage/indexlib/config:SchemaResolver',
        '//aios/storage/indexlib/config:SingleFileCompressConfig',
        '//aios/storage/indexlib/config:TabletOptions',
        '//aios/storage/indexlib/config:TabletSchema',
        '//aios/storage/indexlib/config:UnresolvedSchema',
        '//aios/storage/indexlib/document/document_rewriter:DocumentInfoToAttributeRewriter',
        '//aios/storage/indexlib/index/deletionmap:config',
        '//aios/storage/indexlib/index/inverted_index:constants',
        '//aios/storage/indexlib/index/inverted_index/config:TruncateIndexNameMapper',
        '//aios/storage/indexlib/index/inverted_index/config:TruncateProfileConfig',
        '//aios/storage/indexlib/index/operation_log:config',
        '//aios/storage/indexlib/index/statistics_term:StatisticsTermIndexConfig',
        '//aios/storage/indexlib/index/statistics_term:constants',
        '//aios/storage/indexlib/index/summary:config',
        '//aios/storage/indexlib/legacy/index_base:PartitionMeta',
        '//aios/storage/indexlib/legacy/index_base:SchemaAdapter',
        '//aios/storage/indexlib/table:builtin_define',
        '//aios/storage/indexlib/table/normal_table/virtual_attribute:VirtualAttributeConfig'
    ]
)
indexlib_cc_library(
    name='NormalTabletExporter',
    deps=[
        ':NormalTabletDocIterator',
        '//aios/storage/indexlib/framework:ITabletExporter'
    ]
)
indexlib_cc_library(
    name='NormalTabletValidator',
    deps=[
        ':NormalTabletSessionReader',
        '//aios/storage/indexlib/base:MemoryQuotaController',
        '//aios/storage/indexlib/config:BackgroundTaskConfig',
        '//aios/storage/indexlib/config:TabletOptions',
        '//aios/storage/indexlib/file_system',
        '//aios/storage/indexlib/framework:ITablet',
        '//aios/storage/indexlib/framework:ITabletValidator',
        '//aios/storage/indexlib/framework:TabletCreator',
        '//aios/storage/indexlib/index/primary_key:reader'
    ]
)
indexlib_cc_library(
    name='NormalTabletDocIterator',
    deps=[
        ':NormalTabletSessionReader',
        '//aios/storage/indexlib/document:interface',
        '//aios/storage/indexlib/framework:ITabletDocIterator',
        '//aios/storage/indexlib/framework:MetricsManager',
        '//aios/storage/indexlib/framework:ReadResource',
        '//aios/storage/indexlib/framework:Tablet',
        '//aios/storage/indexlib/framework:TabletCreator',
        '//aios/storage/indexlib/framework:TabletData',
        '//aios/storage/indexlib/index/attribute:reader',
        '//aios/storage/indexlib/index/inverted_index:IndexQueryCondition',
        '//aios/storage/indexlib/index/inverted_index:executor',
        '//aios/storage/indexlib/index/summary:reader'
    ]
)
indexlib_cc_library(
    name='DimensionDescription',
    deps=['//aios/storage/indexlib/index/attribute:constants']
)
indexlib_cc_library(
    name='NormalTabletMetrics',
    deps=[
        '//aios/autil:log', '//aios/storage/indexlib/config:schema',
        '//aios/storage/indexlib/framework:MetricsManager',
        '//aios/storage/indexlib/index/attribute:constants'
    ]
)
indexlib_cc_library(
    name='SegmentSortDecisionMaker',
    deps=[
        '//aios/storage/indexlib/base:Types',
        '//aios/storage/indexlib/config:SortDescription',
        '//aios/storage/indexlib/framework:Segment'
    ]
)
indexlib_cc_library(
    name='NormalDocIdDispatcher',
    deps=['//aios/storage/indexlib/index/primary_key:reader']
)
indexlib_cc_library(
    name='NormalTabletParallelBuilder',
    deps=[
        ':NormalMemSegment', '//aios/storage/indexlib/framework:OpenOptions',
        '//aios/storage/indexlib/index/attribute:AttributeBuildWorkItem',
        '//aios/storage/indexlib/index/common:BuildWorkItem',
        '//aios/storage/indexlib/index/deletionmap:DeletionMapBuildWorkItem',
        '//aios/storage/indexlib/index/inverted_index:InvertedIndexBuildWorkItem',
        '//aios/storage/indexlib/index/operation_log:OperationLogBuildWorkItem',
        '//aios/storage/indexlib/index/primary_key:PrimaryKeyBuildWorkItem',
        '//aios/storage/indexlib/index/primary_key:reader',
        '//aios/storage/indexlib/index/summary:SummaryBuildWorkItem',
        '//aios/storage/indexlib/table/normal_table/virtual_attribute:VirtualAttributeBuildWorkItem',
        '//aios/storage/indexlib/util:thread_pool'
    ]
)
