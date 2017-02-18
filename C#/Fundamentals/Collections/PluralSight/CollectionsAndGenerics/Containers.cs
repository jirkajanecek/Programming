using System.Collections.Generic;

namespace Collections.PluralSight.CollectionsAndGenerics
{
	internal class Containers
	{
		private static readonly string[] sm_payload = {"item3", "item2", "item1", "item0"};

		public System.Collections.Generic.List<string> List { get; } = new System.Collections.Generic.List<string>(sm_payload);

		public Stack<string> Stack { get; } = new Stack<string>();

		public Queue<string> Queue { get; } = new Queue<string>(sm_payload);

		public Dictionary<string, string> Dictionry { get; } = new Dictionary<string, string>()
		{
			[sm_payload[0]] = sm_payload[0],
			[sm_payload[1]] = sm_payload[1],
			[sm_payload[2]] = sm_payload[2],
			[sm_payload[3]] = sm_payload[3]
		};

		public SortedList<string, string> Sortedlist { get; } = new SortedList<string, string>()
		{
			[sm_payload[0]] = sm_payload[0],
			[sm_payload[1]] = sm_payload[1],
			[sm_payload[2]] = sm_payload[2],
			[sm_payload[3]] = sm_payload[3]
		};
	}
}
